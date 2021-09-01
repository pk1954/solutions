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

// When deleting a pipe, also start and endpoint of the pipe are affected
// Case 1: The startpoint (endpoint) has more outgoing (incoming) pipes
//         -> Remove pipe from list of outgoing (incoming) pipes. Done.
// Case 2: Pipe is the only outgoing (incoming) pipe of the startpoint (endpoint)
//      2.1: The startpoint (endpoint) is a knot
//      2.1.1: The knot would be an orphan after deletion of the pipe
//          -> Remove the knot too
//      2.1.2: The knot has remaining connections in the other direction
//         -> Remove pipe from list of outgoing (incoming) pipes. Leave the knot alive.
//      2.2: The startpoint (endpoint) is an ioNeuron, which can exist as orphan
//          -> Remove pipe from list of outgoing (incoming) pipes. Done.
//      2.3: The startpoint (endpoint) is a neuron
//          -> transform the neuron to an output (input) neuron 

class DeletePipeCommand : public Command
{
public:

	DeletePipeCommand(Nob &nob)
	  :	m_pipe(*Cast2Pipe(&nob))
	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		BaseKnot & endKnot   = * m_pipe.GetEndKnotPtr();
		if (startKnot.IsNeuron() && (startKnot.GetNrOfOutgoingConnections() == 1))  //case 2.3: prepare output neuron 
		{
			m_upOutputNeuron = make_unique<OutputNeuron>(startKnot); // gets only incoming pipes from startKnot
			m_upOutputNeuron->SetId(startKnot.GetId());
		}
		if (endKnot.IsNeuron() && (endKnot.GetNrOfIncomingConnections() == 1))    //case 2.3: prepare input neuron
		{
			m_upInputNeuron = make_unique<InputNeuron>(endKnot);  // gets only outgoing pipes from startKnot
			m_upInputNeuron->SetId(endKnot.GetId());
		}
	}

	~DeletePipeCommand(){ }

	virtual void Do()
	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		startKnot.RemoveOutgoing(& m_pipe);
		if (!startKnot.HasOutgoing()) 
		{
			if (startKnot.IsNeuron())                                  // case 2.3: Replace neuron by output neuron
			{
				m_upStartKnot = move(m_pNMWI->ReplaceInModel<OutputNeuron,Neuron>(move(m_upOutputNeuron)));
				m_caseStart = 23;
			}
			else if (startKnot.IsKnot())
			{
				if (!startKnot.HasIncoming())
				{
					m_upStartKnot = m_pNMWI->RemoveFromModel<Knot>(startKnot);  // case 2.1.1
					m_caseStart = 211;
				}
				else
					m_caseStart = 212;
			}
			else 
				m_caseStart = 22;
		}
		else 
			m_caseStart = 1;

		BaseKnot & endKnot = * m_pipe.GetEndKnotPtr();
		endKnot.RemoveIncoming(& m_pipe);
		if (!endKnot.HasIncoming()) 
		{
			if (endKnot.IsNeuron())                                  // case 2.3: Replace neuron by input neuron
			{
				m_upEndKnot = move(m_pNMWI->ReplaceInModel<InputNeuron,Neuron>(move(m_upInputNeuron)));
				m_caseEnd = 23;
			}
			else if (endKnot.IsKnot())
			{
				if (!endKnot.HasOutgoing())      
				{
					m_upEndKnot = m_pNMWI->RemoveFromModel<Knot>(endKnot); // case 2.1.1
					m_caseEnd = 211;
				}
				else
					m_caseEnd = 212;
			}
			else 
				m_caseEnd = 22;
		}
		else
			m_caseEnd = 1;

		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);
	}

	virtual void Undo()
 	{
		m_pNMWI->Restore2Model<Pipe>(move(m_upPipe));

		if (m_upStartKnot)
			m_pNMWI->Restore2Model<BaseKnot>(move(m_upStartKnot));    // case 2.1:
		else if (m_upInputNeuron)
			m_upEndKnot = move(m_pNMWI->ReplaceInModel<Neuron,InputNeuron>(move(m_upInputNeuron)));
		m_pipe.GetStartKnotPtr()->AddOutgoing(& m_pipe);

		if (m_upEndKnot)
			m_pNMWI->Restore2Model<BaseKnot>(move(m_upEndKnot));     // case 2.1:
		else if (m_upOutputNeuron)
			m_upStartKnot = move(m_pNMWI->ReplaceInModel<Neuron,OutputNeuron>(move(m_upOutputNeuron)));
		m_pipe.GetEndKnotPtr()->AddIncoming(& m_pipe);
	}

private:
	int m_caseStart;
	int m_caseEnd;
	Pipe                   & m_pipe;           // reference to original pipe to be removed from model
	unique_ptr<Pipe>         m_upPipe;         // take responsibility for pipe between Do and Undo
	unique_ptr<BaseKnot>     m_upStartKnot;    // responsible for startknot between Do and Undo (cases 2.1 and 2.3)
	unique_ptr<BaseKnot>     m_upEndKnot;      // responsible for endknot   between Do and Undo (cases 2.1 and 2.3)
	unique_ptr<InputNeuron>  m_upInputNeuron;  // input  neuron replacing endknot in case 2.3
	unique_ptr<OutputNeuron> m_upOutputNeuron; // output neuron replacing startknot in case 2.3
};
