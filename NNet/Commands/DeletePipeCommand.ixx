// DeletePipeCommand.ixx
//
// Commands

module;

#include <cassert>
#include <memory>

export module DeletePipeCommand;

import Types;
import SaveCast;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class DeletePipeCommand : public NNetCommand
{
public:

	explicit DeletePipeCommand(Nob & nob)
	:	m_pipe   (*Cast2Pipe(&nob)),
		m_idStart(m_pipe.GetStartNobPtr()->GetId()),
		m_idEnd  (m_pipe.GetEndNobPtr  ()->GetId())
	{}

	~DeletePipeCommand() final = default;

	void Do() final
	{
		m_pNMWI->RemoveOutgoing(m_idStart, m_pipe);
		m_pNMWI->RemoveIncoming(m_idEnd,   m_pipe);

		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);

		if (Nob const * pParent { m_pipe.GetStartNobPtr()->GetParentNob() })
		{
			m_upInputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*pParent);
			m_upInputConnector->ClearParentPointers();
		}
		if (Nob const * pParent { m_pipe.GetEndNobPtr()->GetParentNob() })
		{
			m_upOutputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*pParent);
			m_upOutputConnector->ClearParentPointers();
		}

		m_upStartKnot = fixBaseKnot(m_idStart);
		m_upEndKnot   = fixBaseKnot(m_idEnd);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upStartKnot)); // Restore start/end knot, 
		m_pNMWI->Restore2Model(move(m_upEndKnot));   // if fixBaseKnot had removed

		if (m_upOutputConnector) // restore IoConnector, if neccessary
		{
			m_upOutputConnector->SetParentPointers();
			m_pNMWI->Restore2Model(move(m_upOutputConnector));
		}
		if (m_upInputConnector) // restore IoConnector, if neccessary
		{
			m_upInputConnector->SetParentPointers();
			m_pNMWI->Restore2Model(move(m_upInputConnector));
		}

		m_pNMWI->Restore2Model(move(m_upPipe));  // Restore pipe

		m_pNMWI->AddOutgoing(m_idStart, m_pipe);
		m_pNMWI->AddIncoming(m_idEnd,   m_pipe);

		m_pNMWI->Reconnect(m_idStart);
		m_pNMWI->Reconnect(m_idEnd);

		//m_pNMWI->fixBaseKnot(m_idStart);
		//m_pNMWI->fixBaseKnot(m_idEnd);
	}

private:
	Pipe                  & m_pipe;    // reference to original pipe to be removed from model
	unique_ptr<Pipe>        m_upPipe;  // take ownership of pipe between Do and Undo
	NobId                   m_idStart;
	NobId                   m_idEnd;
	unique_ptr<BaseKnot>    m_upStartKnot;
	unique_ptr<BaseKnot>    m_upEndKnot;
	unique_ptr<IoConnector> m_upInputConnector;
	unique_ptr<IoConnector> m_upOutputConnector;

	// fixBaseKnot: After a pipe deletion a BaseKnot may have changed type
	//              e.g. If a Knot has lost its output pipe, it becomes an OutputLine

	unique_ptr<BaseKnot> fixBaseKnot(NobId const id)
	{
		using enum NobType::Value;

		BaseKnot const * pBaseKnot { m_pNMWI->GetConstBaseKnotPtr(id) };

		if (pBaseKnot == nullptr)
			return unique_ptr<BaseKnot>();

		size_t const nrInPipes  { pBaseKnot->GetNrOfInConns() };
		size_t const nrOutPipes { pBaseKnot->GetNrOfOutConns() };
		NobType      typeNew    { undefined };

		if (pBaseKnot->IsNeuron())
		{
			assert(nrOutPipes == 1);
			if (nrInPipes == 0)	
				typeNew = inputLine;
		}
		else
		{
	             if ((nrOutPipes == 1) && (nrInPipes == 1))	typeNew = knot;
			else if ((nrOutPipes == 0) && (nrInPipes == 1))	typeNew = outputLine;
			else if ((nrOutPipes == 1) && (nrInPipes == 0))	typeNew = inputLine;
		}

		if (typeNew != pBaseKnot->GetNobType())
		{
			unique_ptr<BaseKnot> upBaseKnotNew { BaseKnotFactory::Make(pBaseKnot->GetPos(), typeNew) };
			if (upBaseKnotNew)
			{
				upBaseKnotNew->SetIncoming(* pBaseKnot);
				upBaseKnotNew->SetOutgoing(* pBaseKnot);
				return m_pNMWI->ReplaceInModel<BaseKnot>(move(upBaseKnotNew));
			}
			else 
				return m_pNMWI->RemoveFromModel<BaseKnot>(id);
		}

		return unique_ptr<BaseKnot>();
	}
};
