// DeleteNobCommand.cpp
//
// NNetCommands

module;

#include <cassert>
#include <memory>

module NNetCommands:DeleteNobCommand;

import :DeleteForkOutputCmd;
import :DeleteNeuronInputCmd;
import :DeletePipeCommand;
import :NNetCommand;

using std::move;

class DeleteIoConnectorCmd : public NNetCommand
{
public:
	explicit DeleteIoConnectorCmd(Nob& nob)
		: m_connector(*Cast2IoConnector(&nob))
	{}

	void Do() final
	{
		m_upIoConnector = m_pNMWI->RemoveFromModel<IoConnector>(m_connector);
		m_upIoConnector->DisconnectIoLines();
	}

	void Undo() final
	{
		m_upIoConnector->ConnectIoLines();
		m_pNMWI->Restore2Model(move(m_upIoConnector));
	}

private:

	IoConnector& m_connector;
	unique_ptr<IoConnector> m_upIoConnector;
};

class DeleteKnotCmd : public NNetCommand
{
public:

	explicit DeleteKnotCmd(Nob& nob)
		: m_pKnot(Cast2Knot(&nob))
	{
		m_upInputLine  = make_unique<InputLine >(*m_pKnot->GetOutgoing());
		m_upOutputLine = make_unique<OutputLine>(*m_pKnot->GetIncoming());
	}

	void Do()  final
	{
		m_upKnot = m_pNMWI->RemoveFromModel<Knot>(m_pKnot->GetId());
		m_upKnot->GetOutgoing()->SetStartPnt(m_upInputLine.get());
		m_upKnot->GetIncoming()->SetEndPnt(m_upOutputLine.get());
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_pNMWI->Push2Model(move(m_upOutputLine));
	}

	void Undo() final
	{
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_upInputLine  = m_pNMWI->PopFromModel<InputLine>();
		m_upOutputLine->GetPipe()->SetEndPnt(m_upKnot.get());
		m_upInputLine->GetPipe()->SetStartPnt(m_upKnot.get());
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
		: m_pFork(Cast2Fork(&nob))
	{
		m_upDeleteForkOutputCmd = make_unique<DeleteForkOutputCmd>(m_pFork);
		m_upOutputLine = make_unique<OutputLine>(*(m_pFork->GetIncoming()));
	}

	void Do() final
	{
		m_pFork->GetIncoming()->SetEndPnt(m_upOutputLine.get());
		m_pNMWI->Push2Model(move(m_upOutputLine));
		m_upDeleteForkOutputCmd->Do();
	}

	void Undo() final
	{
		m_upDeleteForkOutputCmd->Undo();
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_upOutputLine->GetPipe()->SetEndPnt(m_pFork);
	}

private:
	Fork* m_pFork;
	unique_ptr<OutputLine>          m_upOutputLine;
	unique_ptr<DeleteForkOutputCmd> m_upDeleteForkOutputCmd;
};

class DeleteSynapseCmd : public NNetCommand
{
public:

	explicit DeleteSynapseCmd(Nob& nob)
	{
		Synapse* pSynapse { Cast2Synapse(&nob) };
		m_upOutputLine = make_unique<OutputLine>(*pSynapse->GetAddPipe());
		m_upKnot       = make_unique<Knot>(*pSynapse);
		m_upKnot->AddIncoming(pSynapse->GetInPipe());
		m_upKnot->AddOutgoing(pSynapse->GetOutPipe());
	}

	void Do()  final
	{
		m_upKnot->GetIncoming()->SetEndPnt  (m_upKnot.get());
		m_upKnot->GetOutgoing()->SetStartPnt(m_upKnot.get());
		m_upSynapse = m_pNMWI->ReplaceInModel<Synapse>(move(m_upKnot));
		m_upSynapse->GetAddPipe()->SetEndPnt(m_upOutputLine.get());
		m_pNMWI->Push2Model(move(m_upOutputLine));
	}

	void Undo() final
	{
		Synapse * pSynapse { m_upSynapse.get() };
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_upOutputLine->GetPipe()->SetEndPnt(pSynapse);
		m_upKnot = m_pNMWI->ReplaceInModel<Knot>(move(m_upSynapse));
		m_upKnot->GetOutgoing()->SetStartPnt(pSynapse);
		m_upKnot->GetIncoming()->SetEndPnt  (pSynapse);
	}

private:
	unique_ptr<Knot>       m_upKnot;
	unique_ptr<Synapse>    m_upSynapse;
	unique_ptr<OutputLine> m_upOutputLine;
};

class DeleteNeuronCmd : public NNetCommand
{
public:

	explicit DeleteNeuronCmd(Nob& nob)
		: m_pNeuron(Cast2Neuron(&nob))
	{
		m_upDeleteNeuronInputCmd = make_unique<DeleteNeuronInputCmd>(m_pNeuron);
		m_upInputLine = make_unique<InputLine>(*m_pNeuron->GetAxon());
	}

	void Do() final
	{
		Pipe* pAxon { m_pNeuron->GetAxon() };
		pAxon->SetStartPnt(m_upInputLine.get());
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_upDeleteNeuronInputCmd->Do();
	}

	void Undo() final
	{
		Pipe* pAxon { m_pNeuron->GetAxon() };
		pAxon->SetStartPnt(m_pNeuron);
		m_upDeleteNeuronInputCmd->Undo();
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
	}

private:
	Neuron* m_pNeuron;
	unique_ptr<InputLine>            m_upInputLine;
	unique_ptr<DeleteNeuronInputCmd> m_upDeleteNeuronInputCmd;
};

unique_ptr<NNetCommand> DeleteNobCommand::MakeCommand(NobId const id)
{
	unique_ptr<NNetCommand> upCmd;
	Nob& nob { *m_pNMWI->GetNob(id) };
	if (m_pNMWI->IsNobInModel(nob) && !nob.HasParentNob())
	{
		switch (nob.GetNobType().GetValue())
		{
			using enum NobType::Value;
		case inputConnector:
		case outputConnector: upCmd = make_unique<DeleteIoConnectorCmd>(nob); break;
		case pipe:		      upCmd = make_unique<DeletePipeCommand>   (nob); break;
		case knot:            upCmd = make_unique<DeleteKnotCmd>       (nob); break;
		case fork:            upCmd = make_unique<DeleteForkCmd>       (nob); break;
		case synapse:         upCmd = make_unique<DeleteSynapseCmd>    (nob); break;
		case neuron:          upCmd = make_unique<DeleteNeuronCmd>     (nob); break;
		case outputLine:      break;  // Output line cannot be deleted. Delete Pipe!
		case inputLine:    	  break;  // Input  line cannot be deleted. Delete Pipe!
		default:              assert(false);
		}
	}
	return upCmd;
}
