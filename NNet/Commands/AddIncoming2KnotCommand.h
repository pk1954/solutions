// AddIncoming2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class AddIncoming2KnotCommand : public Command
{
public:
	AddIncoming2KnotCommand( NNetModel * pModel, ShapeId const id, MicroMeterPoint const & pos )
	{ 
		m_pKnotNew = pModel->NewBaseKnot<Knot>( pos );
		m_pEnd     = pModel->GetShapePtr<BaseKnot *>( id );
		m_pPipe    = pModel->NewPipe( m_pEnd, m_pKnotNew );
		m_pKnotNew->m_connections.AddOutgoing( m_pPipe );
	}

	~AddIncoming2KnotCommand( )
	{
		delete m_pKnotNew;
		delete m_pPipe;
	}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		m_pEnd->m_connections.AddIncoming( m_pPipe );
		pModel->Store2Model( m_pKnotNew );
		pModel->Store2Model( m_pPipe );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		m_pEnd->m_connections.RemoveIncoming( m_pPipe );
		pModel->RemoveFromModel( m_pKnotNew );
		pModel->RemoveFromModel( m_pPipe );
	}

private:
	Knot     * m_pKnotNew;
	BaseKnot * m_pEnd;
	Pipe     * m_pPipe;
};

