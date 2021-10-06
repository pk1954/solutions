// AddOutgoing2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

class AddOutgoing2KnotCommand : public NNetCommand
{
public:
	AddOutgoing2KnotCommand
	(
		NobId         const   id, 
		MicroMeterPnt const & pos 
	)
	  :	m_idKnot(id),
		m_pos(pos)
	{ 
		m_pStart    = m_pNMWI->GetNobPtr<BaseKnot *>(m_idKnot);
		m_upKnotNew = make_unique<Knot>(m_pos);
		m_upPipe    = make_unique<Pipe>(m_pStart, m_upKnotNew.get());
		m_upKnotNew->AddIncoming(m_upPipe.get());
	}

	~AddOutgoing2KnotCommand() {}

	virtual void Do() 
	{ 
		m_pStart->AddOutgoing(m_upPipe.get());
		m_pNMWI->Push2Model(move(m_upKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
	}

	virtual void Undo() 
	{ 
		m_upPipe    = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotNew = m_pNMWI->PopFromModel<Knot>();
		m_pStart->RemoveOutgoing(m_upPipe.get());
	}

private:
	BaseKnot          * m_pStart    { nullptr };
	unique_ptr<Knot>    m_upKnotNew { nullptr };
	unique_ptr<Pipe>    m_upPipe    { nullptr };
	NobId         const m_idKnot;
	MicroMeterPnt const m_pos; 
};
