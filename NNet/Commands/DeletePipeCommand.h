// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "IoConnector.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;

class DeletePipeCommand : public NNetCommand
{
public:

	explicit DeletePipeCommand(Nob &nob)
		:	m_pipe     (*Cast2Pipe(&nob)),
		    m_startKnot(* m_pipe.GetStartKnotPtr()),
            m_endKnot  (* m_pipe.GetEndKnotPtr()),
	        m_idStart  (m_startKnot.GetId()),
		    m_idEnd    (m_endKnot.GetId())
	{}

	~DeletePipeCommand() final = default;

	void Do() final
	{
		m_pNMWI->RemoveOutgoing(m_idStart, m_pipe);
		m_pNMWI->RemoveIncoming(m_idEnd,   m_pipe);
		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);
		if (m_startKnot.HasParentNob())
		{
			m_upInputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*m_startKnot.GetParentNob());
			m_upInputConnector->ClearParentPointers();
		}
		if (m_endKnot.HasParentNob())
		{
			m_upOutputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*m_endKnot.GetParentNob());
			m_upOutputConnector->ClearParentPointers();
		}
		m_upStartKnot = m_pNMWI->FixBaseKnot(m_idStart);
		m_upEndKnot   = m_pNMWI->FixBaseKnot(m_idEnd);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upStartKnot));
		m_pNMWI->Restore2Model(move(m_upEndKnot));
		if (m_upOutputConnector)
		{
			m_upOutputConnector->SetParentPointers();
			m_pNMWI->Restore2Model(move(m_upOutputConnector));
		}
		if (m_upInputConnector)
		{
			m_upInputConnector->SetParentPointers();
			m_pNMWI->Restore2Model(move(m_upInputConnector));
		}
		m_pNMWI->Restore2Model(move(m_upPipe));

		m_pNMWI->AddOutgoing(m_idStart, m_pipe);
		m_pNMWI->AddIncoming(m_idEnd,   m_pipe);

		m_pNMWI->Reconnect(m_idStart);
		m_pNMWI->Reconnect(m_idEnd);

		m_pNMWI->FixBaseKnot(m_idStart);
		m_pNMWI->FixBaseKnot(m_idEnd);
	}

private:
	Pipe                  & m_pipe;    // reference to original pipe to be removed from model
	BaseKnot              & m_startKnot;
	BaseKnot              & m_endKnot;
	unique_ptr<Pipe>        m_upPipe;  // take ownership of pipe between Do and Undo
	NobId                   m_idStart;
	NobId                   m_idEnd;
	unique_ptr<BaseKnot>    m_upStartKnot;
	unique_ptr<BaseKnot>    m_upEndKnot;
	unique_ptr<IoConnector> m_upInputConnector;
	unique_ptr<IoConnector> m_upOutputConnector;
};
