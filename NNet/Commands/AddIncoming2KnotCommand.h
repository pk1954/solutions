// AddIncoming2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "BaseKnot.h"
#include "Knot.h"

class AddIncoming2KnotCommand : public NNetCommand
{
public:
	AddIncoming2KnotCommand
	(
		NobId         const   id, 
		MicroMeterPnt const & pos 
	)
	  :	m_idKnot(id),
		m_pos(pos)
	{ 
		m_pEnd      = m_pNMWI->GetNobPtr<BaseKnot *>(m_idKnot);
		m_upKnotNew = make_unique<Knot>(m_pos);
		m_upPipe    = make_unique<Pipe>(m_upKnotNew.get(), m_pEnd);
		m_upKnotNew->AddOutgoing(m_upPipe.get());
	}
	
	~AddIncoming2KnotCommand()	{ }

	void Do() final 
	{ 
		m_pEnd->AddIncoming(m_upPipe.get());
		m_pNMWI->Push2Model(move(m_upKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
	}

	void Undo() final 
	{ 
		m_upPipe    = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotNew = m_pNMWI->PopFromModel<Knot>();
		m_pEnd->RemoveIncoming(m_upPipe.get());
	}

private:
	BaseKnot          * m_pEnd      { nullptr };
	unique_ptr<Knot>    m_upKnotNew { nullptr };
	unique_ptr<Pipe>    m_upPipe    { nullptr };
	NobId         const m_idKnot;
	MicroMeterPnt const m_pos; 
};

