// AddOutgoing2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

class AddOutgoing2KnotCommand : public Command
{
public:
	AddOutgoing2KnotCommand
	( 
		NobId           const   id, 
		MicroMeterPnt const & pos 
	)
	  :	m_idKnot(id),
		m_pos(pos)
	{ }

	~AddOutgoing2KnotCommand() {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_upPipe )
		{
			m_pStart    = nmwi.GetNobPtr<BaseKnot *>( m_idKnot );
			m_upKnotNew = make_unique<Knot>( m_pos );
			m_upPipe    = make_unique<Pipe>( m_pStart, m_upKnotNew.get() );
			m_upKnotNew->m_connections.AddIncoming( m_upPipe.get() );
		}
		m_pStart->m_connections.AddOutgoing( m_upPipe.get() );
		nmwi.GetUPNobs().Push( move( m_upKnotNew ) );
		nmwi.GetUPNobs().Push( move( m_upPipe ) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upPipe    = nmwi.GetUPNobs().Pop<Pipe>();
		m_upKnotNew = nmwi.GetUPNobs().Pop<Knot>();
		m_pStart->m_connections.RemoveOutgoing( m_upPipe.get() );
	}

private:
	BaseKnot      *       m_pStart    { nullptr };
	unique_ptr<Knot>      m_upKnotNew { nullptr };
	unique_ptr<Pipe>      m_upPipe    { nullptr };
	NobId         const m_idKnot;
	MicroMeterPnt const m_pos; 
};
