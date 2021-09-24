// AddIncoming2PipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "BaseKnot.h"

class AddIncoming2PipeCommand : public NNetCommand
{
public:
	AddIncoming2PipeCommand
	(
		NobId         const   idPipe, 
		MicroMeterPnt const & pos 
	)
	  :	m_idPipe(idPipe),
		m_pos(pos)
	{ 
		m_pPipeOld      = m_pNMWI->GetNobPtr<Pipe *>(m_idPipe);
		m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr();
		m_upKnotInsert  = make_unique<Knot>(m_pos);                                
		m_upKnotInsert->Select(m_pPipeOld->IsSelected());
		m_upKnotOrtho   = make_unique<Knot>(m_pos - m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho   = make_unique<Pipe>(m_upKnotOrtho.get(), m_upKnotInsert.get());		
		m_upPipeExt     = make_unique<Pipe>(m_pStartKnotOld,     m_upKnotInsert.get());   	

		m_upKnotInsert->AddIncoming(m_upPipeExt.get());
		m_upKnotOrtho ->AddOutgoing(m_upPipeOrtho.get());
		m_upKnotInsert->AddIncoming(m_upPipeOrtho.get());
		m_upKnotInsert->AddOutgoing(m_pPipeOld);
	}
	
	~AddIncoming2PipeCommand()	{ }

	virtual void Do() 
	{ 
		m_pStartKnotOld->ReplaceOutgoing(m_pPipeOld, m_upPipeExt.get());
		m_pPipeOld->SetStartKnot(m_upKnotInsert.get());
		m_pNMWI->Push2Model(move(m_upKnotOrtho));
		m_pNMWI->Push2Model(move(m_upKnotInsert));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upPipeExt));
	}

	virtual void Undo() 
	{ 
		m_upPipeExt    = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeOrtho  = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotInsert = m_pNMWI->PopFromModel<Knot>();
		m_upKnotOrtho  = m_pNMWI->PopFromModel<Knot>();
		m_pPipeOld->SetStartKnot(m_pStartKnotOld);
		m_pStartKnotOld->ReplaceOutgoing(m_upPipeExt.get(), m_pPipeOld);
	}

private:
	Pipe     *       m_pPipeOld      { nullptr };
	BaseKnot *       m_pStartKnotOld { nullptr };
	unique_ptr<Pipe> m_upPipeExt     { nullptr };
	unique_ptr<Pipe> m_upPipeOrtho   { nullptr };
	unique_ptr<Knot> m_upKnotInsert  { nullptr };
	unique_ptr<Knot> m_upKnotOrtho   { nullptr };

	NobId          const m_idPipe;
	MicroMeterPnt  const m_pos; 
};
