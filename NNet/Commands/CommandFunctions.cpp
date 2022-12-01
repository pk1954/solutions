// CommandFunctions.cpp
//
// Commands

module;

#include <cassert>
#include <memory>
#include <vector>

module CommandFunctions;

import DeleteForkOutputCmd;
import DeleteNeuronInputCmd;
import DeleteIoConnectorCmd;
import DeletePipeCommand;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;
using std::vector;

class DeleteKnotCmd : public NNetCommand
{
public:

	explicit DeleteKnotCmd(Nob& nob)
		: m_pKnot(Cast2Knot(&nob))
	{
		m_upInputLine  = make_unique<InputLine> (*m_pKnot);
		m_upOutputLine = make_unique<OutputLine>(*m_pKnot->GetIncoming());
		m_upInputLine ->SetPipe(m_pKnot->GetOutgoing());
	}

	void Do()  final
	{
		m_upKnot = m_pNMWI->RemoveFromModel<Knot>(m_pKnot->GetId());
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_pNMWI->Push2Model(move(m_upOutputLine));
	}

	void Undo() final
	{
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
		m_pNMWI->Restore2Model(move(m_upKnot));
	}

private:
	Knot* m_pKnot;
	unique_ptr<Knot>       m_upKnot;
	unique_ptr<InputLine>  m_upInputLine;
	unique_ptr<OutputLine> m_upOutputLine;
};

class DeleteForkCmd : public NNetCommand
{
public:

	explicit DeleteForkCmd(Nob& nob)
	{
		m_id = nob.GetId();
		Fork * pFork { Cast2Fork(&nob) };
		m_upDeleteForkOutputCmd = make_unique<DeleteForkOutputCmd>(m_id);
		m_upOutputLine = make_unique<OutputLine>(*pFork->GetIncoming());
	}

	void Do()  final
	{
		m_upFork = m_pNMWI->RemoveFromModel<Fork>(m_id);
		m_pNMWI->Push2Model(move(m_upOutputLine));
		m_upDeleteForkOutputCmd->Do();
	}

	void Undo() final
	{
		m_upDeleteForkOutputCmd->Undo();
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_pNMWI->Restore2Model(move(m_upFork));
	}

private:
	NobId                           m_id;
	unique_ptr<Fork>                m_upFork;
	unique_ptr<OutputLine>          m_upOutputLine;
	unique_ptr<DeleteForkOutputCmd> m_upDeleteForkOutputCmd;
};


class DeleteNeuronCmd : public NNetCommand
{
public:

	explicit DeleteNeuronCmd(Nob& nob)
	  :	m_id(nob.GetId())
	{
		m_upDeleteNeuronInputCmd = make_unique<DeleteNeuronInputCmd>(m_id);
		m_upInputLine            = make_unique<InputLine>(*m_pNMWI->GetNobPtr<Neuron*>(m_id));
		m_upInputLine->SetPipe(m_upNeuron->GetAxon());
	}

	void Do() final
	{
		m_upNeuron = m_pNMWI->RemoveFromModel<Neuron>(m_id);
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_upDeleteNeuronInputCmd->Do();
	}

	void Undo() final
	{
		m_upDeleteNeuronInputCmd->Undo();
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
		m_pNMWI->Restore2Model(move(m_upNeuron));
	}

private:
	NobId                            m_id;
	unique_ptr<Neuron>               m_upNeuron;
	unique_ptr<InputLine>            m_upInputLine;
	unique_ptr<DeleteNeuronInputCmd> m_upDeleteNeuronInputCmd;
};

unique_ptr<NNetCommand> MakeDeleteCommand
(
	NNetModelWriterInterface const & nmwi,
	Nob                            & nob
)
{
	unique_ptr<NNetCommand> upCmd;
	if (nmwi.IsNobInModel(nob) && !nob.HasParentNob())
		switch (nob.GetNobType().GetValue())
		{
		using enum NobType::Value;
		case inputConnector:
		case outputConnector: upCmd = make_unique<DeleteIoConnectorCmd>(nob); break;
		case pipe:		      upCmd = make_unique<DeletePipeCommand>   (nob); break;
		case knot:            upCmd = make_unique<DeleteKnotCmd>       (nob); break;
		case fork:            upCmd = make_unique<DeleteForkCmd>       (nob); break;
		case neuron:          upCmd = make_unique<DeleteNeuronCmd>     (nob); break;
		case outputLine:   	break;  // Output line cannot be deleted. Delete Pipe!
		case inputLine:    	break;  // Input  line cannot be deleted. Delete Pipe!
		default:              assert(false);
		}
	return move(upCmd);
}
