// AddOutgoing2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

class AddOutgoing2KnotCommand : public Command
{
public:
	AddOutgoing2KnotCommand
	( 
		NNetModelWriterInterface * pModel, 
		ShapeId            const   id, 
		MicroMeterPoint    const & pos 
	)
	{ 
		m_pStart   = pModel->GetShapePtr<BaseKnot *>( id );
		m_pKnotNew = pModel->NewBaseKnot<Knot>( pos );
		m_pPipe    = pModel->NewPipe( m_pStart, m_pKnotNew );
		m_pKnotNew->m_connections.AddIncoming( m_pPipe );
	}

	~AddOutgoing2KnotCommand( )
	{
		delete m_pKnotNew;
		delete m_pPipe;
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pStart->m_connections.AddOutgoing( m_pPipe );
		pModel->Store2Model( m_pKnotNew );
		pModel->Store2Model( m_pPipe );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pStart->m_connections.RemoveOutgoing( m_pPipe );
		pModel->RemoveFromModel( m_pKnotNew );
		pModel->RemoveFromModel( m_pPipe );
	}

private:
	Knot     * m_pKnotNew;
	BaseKnot * m_pStart;
	Pipe     * m_pPipe;
};
