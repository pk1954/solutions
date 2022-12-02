// DeletePipeCommand.ixx
//
// Commands

module;

#include <cassert>
#include <memory>
#include <vector>

export module DeletePipeCommand;

import Types;
import SaveCast;
import NNetCommand;
import NNetModel;
import DeleteNeuronInputCmd;
import DeleteForkOutputCmd;

using std::unique_ptr;
using std::make_unique;
using std::vector;

template <Nob_t T>
class RemoveNobCmd : public NNetCommand
{
public:

	explicit RemoveNobCmd(NobId const id)
		: m_id(id)
	{}

	void Do()   final { m_upNob = m_pNMWI->RemoveFromModel<T>(m_id); }
	void Undo() final {	m_pNMWI->Restore2Model(move(m_upNob)); }

private:
	NobId         m_id;
	unique_ptr<T> m_upNob;
};

template <Nob_t OLD, Nob_t NEW>
class ReplaceNobCmd : public NNetCommand
{
public:

	explicit ReplaceNobCmd(NobId const id)
	{
		m_upNew = make_unique<NEW>(*m_pNMWI->GetNobPtr<OLD*>(id));
	}

	void Do()   final { m_upOld = m_pNMWI->ReplaceInModel<OLD>(move(m_upNew)); }
	void Undo() final { m_upNew = m_pNMWI->ReplaceInModel<NEW>(move(m_upOld)); }

private:
	unique_ptr<OLD> m_upOld;
	unique_ptr<NEW> m_upNew;
};

class ForkStartCmd : public NNetCommand
{
public:

	ForkStartCmd(Pipe& pipe, NobId const id)
	{
		Fork* pFork { m_pNMWI->GetNobPtr<Fork*>(id) };
		m_upKnot = make_unique<Knot>(*pFork);
		m_upKnot->AddIncoming(pFork->GetIncoming());
		m_upKnot->AddOutgoing(pFork->GetOtherOutgoing(&pipe));
	}

	void Do()   final { m_upFork = m_pNMWI->ReplaceInModel<Fork>(move(m_upKnot)); }
	void Undo() final { m_upKnot = m_pNMWI->ReplaceInModel<Knot>(move(m_upFork)); }

private:
	unique_ptr<Fork> m_upFork;
	unique_ptr<Knot> m_upKnot;
};

class SynapseEndCmd : public NNetCommand
{
public:

	explicit SynapseEndCmd(NobId const id)
		: m_id(id)
	{}

	void Do() final
	{
		m_upSynapse = m_pNMWI->RemoveFromModel<Synapse>(m_id);
		m_upSynapse->RemoveFromMainPipe();
	}

	void Undo() final
	{
		m_upSynapse->Add2MainPipe();
		m_pNMWI->Restore2Model(move(m_upSynapse));
	}

private:
	NobId               m_id;
	unique_ptr<Synapse> m_upSynapse;
};

class SynapseMainCmd : public NNetCommand
{
public:

	explicit SynapseMainCmd(NobId const id)
		: m_id(id)
	{}

	void Do() final
	{
		m_upSynapse = m_pNMWI->RemoveFromModel<Synapse>(m_id);
		m_upSynapse->RemoveFromMainPipe();
	}

	void Undo() final
	{
		m_upSynapse->Add2MainPipe();
		m_pNMWI->Restore2Model(move(m_upSynapse));
	}

private:
	NobId               m_id;
	unique_ptr<Synapse> m_upSynapse;
};

class NeuronEndMultipleCmd : public NNetCommand
{
public:

    NeuronEndMultipleCmd(Pipe& pipe, NobId const id)
	  : m_pipe(pipe)
	{
		m_pNeuron = m_pNMWI->GetNobPtr<Neuron*>(id);
	}

	void Do()   final { m_pNeuron->RemoveIncoming(&m_pipe); }
	void Undo() final {	m_pNeuron->AddIncoming   (&m_pipe);	}

private:
	Pipe   & m_pipe;
	Neuron * m_pNeuron;
};

export class DeletePipeCommand : public NNetCommand
{
	using enum NobType::Value;
public:
	explicit DeletePipeCommand(Nob & nob)
	:	m_pipe   (*Cast2Pipe(&nob)),
		m_idStart(m_pipe.GetStartNobPtr()->GetId()),
		m_idEnd  (m_pipe.GetEndNobPtr  ()->GetId())
	{
		switch (m_pNMWI->GetNobType(m_idStart).GetValue())
		{
			case inputLine: m_upCmdStart = make_unique<RemoveNobCmd<InputLine>>        (m_idStart);	break;
			case knot:      m_upCmdStart = make_unique<ReplaceNobCmd<Knot, OutputLine>>(m_idStart);	break;
			case fork:      m_upCmdStart = make_unique<ForkStartCmd>           (m_pipe, m_idStart);	break;
			case neuron:    m_upCmdStart = make_unique<DeleteNeuronInputCmd>           (m_idStart);	break;
			default:		assert(false);
		}

		switch (m_pNMWI->GetNobType(m_idEnd).GetValue())
		{
			case outputLine: m_upCmdEnd = make_unique<RemoveNobCmd<OutputLine>>      (m_idEnd);	break;
			case knot:       m_upCmdEnd = make_unique<ReplaceNobCmd<Knot, InputLine>>(m_idEnd); break;
			case fork:       m_upCmdEnd = make_unique<DeleteForkOutputCmd>           (m_idEnd); break;
			case synapse:    m_upCmdEnd = make_unique<SynapseEndCmd>                 (m_idEnd); break;
			case neuron:     if (m_pNMWI->GetNobPtr<Neuron*>(m_idEnd)->GetNrOfInConns() == 1)
				                 m_upCmdEnd = make_unique<ReplaceNobCmd<Neuron, InputLine>>(m_idEnd);
					         else
				                 m_upCmdEnd = make_unique<NeuronEndMultipleCmd>(m_pipe, m_idEnd);
							 break;
			default:         assert(false);
		}

		m_pipe.Apply2AllSynapses
		(
			[this](Nob * pNob)
			{
				m_cmdStack.Push(make_unique<ReplaceNobCmd<Synapse, OutputLine>>(pNob->GetId()));
			}
		);
	}

	~DeletePipeCommand() final = default;

	void Do() final
	{
		m_upCmdStart->Do();
		m_upCmdEnd  ->Do();
		m_cmdStack.DoAll();

		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);

		//if (Nob const * pParent { m_pipe.GetStartNobPtr()->GetParentNob() })
		//{
		//	m_upInputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*pParent);
		//	m_upInputConnector->ClearParentPointers();
		//}
		//if (Nob const * pParent { m_pipe.GetEndNobPtr()->GetParentNob() })
		//{
		//	m_upOutputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*pParent);
		//	m_upOutputConnector->ClearParentPointers();
		//}
		m_pNMWI->CheckModel();
	}

	void Undo() final
	{
		m_upCmdStart->Undo();
		m_upCmdEnd  ->Undo();
		m_cmdStack.UndoAll();

		//m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);

		//if (Nob const* pParent { m_upPipe->GetStartNobPtr()->GetParentNob() })
		//{
		//	m_upInputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*pParent);
		//	m_upInputConnector->ClearParentPointers();
		//}
		//if (Nob const* pParent { m_upPipe->GetEndNobPtr()->GetParentNob() })
		//{
		//	m_upOutputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*pParent);
		//	m_upOutputConnector->ClearParentPointers();
		//}
		//if (m_upOutputConnector) // restore IoConnector, if neccessary
		//{
		//	m_upOutputConnector->SetParentPointers();
		//	m_pNMWI->Restore2Model(move(m_upOutputConnector));
		//}
		//if (m_upInputConnector) // restore IoConnector, if neccessary
		//{
		//	m_upInputConnector->SetParentPointers();
		//	m_pNMWI->Restore2Model(move(m_upInputConnector));
		//}

		m_pNMWI->Restore2Model(move(m_upPipe));  // Restore pipe

		//m_pNMWI->Reconnect(m_idStart);  //TODO: Still needed?
		//m_pNMWI->Reconnect(m_idEnd);    //TODO: Still needed?
		m_pNMWI->CheckModel();
	}

private:
	Pipe                  & m_pipe;    // reference to original pipe to be removed from model
	unique_ptr<Pipe>        m_upPipe;  // take ownership of pipe between Do and Undo

	NobId                   m_idStart;
	NobId                   m_idEnd;

	unique_ptr<IoConnector> m_upInputConnector;
	unique_ptr<IoConnector> m_upOutputConnector;

	unique_ptr<NNetCommand> m_upCmdStart;
	unique_ptr<NNetCommand> m_upCmdEnd;

	CommandStack m_cmdStack {};
};
