// AddIncoming2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"

class AddIncoming2KnotCommand : public Command
{
public:
	AddIncoming2KnotCommand
	( 
		NobId         const   id, 
		MicroMeterPnt const & pos 
	)
	  :	m_idKnot(id),
		m_pos(pos)
	{ }
	
	~AddIncoming2KnotCommand()	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_upPipe )
		{
			m_pEnd      = nmwi.GetNobPtr<BaseKnot *>( m_idKnot );
			m_upKnotNew = make_unique<Knot>( m_pos );
			m_upPipe    = make_unique<Pipe>( m_upKnotNew.get(), m_pEnd );
			m_upKnotNew->AddOutgoing( m_upPipe.get() );
		}
		m_pEnd->AddIncoming( m_upPipe.get() );
		nmwi.Push2Model( move(m_upKnotNew) );
		nmwi.Push2Model( move(m_upPipe) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upPipe    = nmwi.PopFromModel<Pipe>();
		m_upKnotNew = nmwi.PopFromModel<Knot>();
		m_pEnd->RemoveIncoming( m_upPipe.get() );
	}

private:
	BaseKnot          * m_pEnd      { nullptr };
	unique_ptr<Knot>    m_upKnotNew { nullptr };
	unique_ptr<Pipe>    m_upPipe    { nullptr };
	NobId         const m_idKnot;
	MicroMeterPnt const m_pos; 
};

