// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;

class DeletePipeCommand : public Command
{
public:

	DeletePipeCommand(Nob &nob)
	  :	m_pipe(*Cast2Pipe(&nob))
	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		BaseKnot & endKnot   = * m_pipe.GetEndKnotPtr();
		if (startKnot.IsNeuron() && (startKnot.GetNrOfOutgoingConnections() == 1))
		{
			m_upOutputNeuron = make_unique<OutputNeuron>(startKnot);
			m_upOutputNeuron->SetId(startKnot.GetId());
		}
		if (endKnot.IsNeuron() && (endKnot.GetNrOfIncomingConnections() == 1))
		{
			m_upInputNeuron = make_unique<InputNeuron>(endKnot);
			m_upInputNeuron->SetId(endKnot.GetId());
		}
	}

	~DeletePipeCommand(){ }

	virtual void Do()
	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		startKnot.RemoveOutgoing(& m_pipe);
		if (startKnot.IsOrphan() && ! startKnot.IsIoNeuron())
			m_upStartKnot = m_pNMWI->RemoveFromModel<Knot>(startKnot);
		if (m_upOutputNeuron)
			m_upStartKnot = move(m_pNMWI->ReplaceInModel<OutputNeuron,Neuron>(move(m_upOutputNeuron)));

		BaseKnot & endKnot = * m_pipe.GetEndKnotPtr();
		endKnot.RemoveIncoming(& m_pipe);
		if (endKnot.IsOrphan() && ! endKnot.IsIoNeuron())
			m_upEndKnot = m_pNMWI->RemoveFromModel<Knot>(endKnot);
		if (m_upInputNeuron)
			m_upEndKnot = move(m_pNMWI->ReplaceInModel<InputNeuron,Neuron>(move(m_upInputNeuron)));

		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);
	}

	virtual void Undo()
 	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		startKnot.AddOutgoing(& m_pipe);
		if (m_upStartKnot)
			m_pNMWI->Restore2Model<BaseKnot>(move(m_upStartKnot));

		BaseKnot & endKnot = * m_pipe.GetEndKnotPtr();
		endKnot.AddIncoming(& m_pipe);
		if (m_upEndKnot)
			m_pNMWI->Restore2Model<BaseKnot>(move(m_upEndKnot));

		if (m_upInputNeuron)
			m_upEndKnot = move(m_pNMWI->ReplaceInModel<Neuron,InputNeuron>(move(m_upInputNeuron)));
		if (m_upOutputNeuron)
			m_upStartKnot = move(m_pNMWI->ReplaceInModel<Neuron,OutputNeuron>(move(m_upOutputNeuron)));

		m_pNMWI->Restore2Model<Pipe>(move(m_upPipe));
	}

private:
	Pipe                   & m_pipe;
	unique_ptr<Pipe>         m_upPipe;
	unique_ptr<BaseKnot>     m_upStartKnot;
	unique_ptr<BaseKnot>     m_upEndKnot;
	unique_ptr<InputNeuron>  m_upInputNeuron;
	unique_ptr<OutputNeuron> m_upOutputNeuron;
};
