// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;

class DeletePipeCommand : public NNetCommand
{
public:

	explicit DeletePipeCommand(Nob &nob)
		:	m_pipe(*Cast2Pipe(&nob)),
			m_idStart(m_pipe.GetStartKnotId()),
		    m_idEnd  (m_pipe.GetEndKnotId())
	{}

	~DeletePipeCommand() final = default;

	void Do() final
	{
		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_pipe);
		m_pNMWI->GetNobPtr<BaseKnot *>(m_idStart)->RemoveOutgoing(m_upPipe.get());
		m_pNMWI->GetNobPtr<BaseKnot *>(m_idEnd)  ->RemoveIncoming(m_upPipe.get());
		m_upStartKnot = m_pNMWI->FixBaseKnot(m_idStart);
		m_upEndKnot   = m_pNMWI->FixBaseKnot(m_idEnd);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upStartKnot));
		m_pNMWI->Restore2Model(move(m_upEndKnot));
		m_pNMWI->GetNobPtr<BaseKnot *>(m_idStart)->AddOutgoing(m_upPipe.get());
		m_pNMWI->GetNobPtr<BaseKnot *>(m_idEnd)  ->AddIncoming(m_upPipe.get());
		m_pNMWI->Restore2Model(move(m_upPipe));
		m_pNMWI->FixBaseKnot(m_idStart);
		m_pNMWI->FixBaseKnot(m_idEnd);
	}

private:
	Pipe               & m_pipe;        // reference to original pipe to be removed from model
	unique_ptr<Pipe>     m_upPipe;      // take responsibility for pipe between Do and Undo
	NobId                m_idStart;
	NobId                m_idEnd;
	unique_ptr<BaseKnot> m_upStartKnot;
	unique_ptr<BaseKnot> m_upEndKnot;
};

