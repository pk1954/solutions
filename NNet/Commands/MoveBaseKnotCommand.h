// MoveBaseKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MoveCommand.h"
#include "BaseKnot.h"

class MoveBaseKnotCommand : public MoveCommand
{
public:
	MoveBaseKnotCommand
	( 
		NNetModelWriterInterface & model, 
		ShapeId            const   id, 
		MicroMeterPoint    const & delta 
	)
	  : MoveCommand( delta ),
		m_pBaseKnot( model.GetShapePtr<BaseKnot *>( id ) )
	{
		m_posBaseKnotOld = m_pBaseKnot->GetPosition();
		wcout << L"# MoveBaseKnotCommand: old pos = " << m_posBaseKnotOld << endl; 
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld + m_delta );
		wcout << L"# MoveBaseKnotCommand.Do: pos = " << m_posBaseKnotOld + m_delta << endl; 
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld );
		wcout << L"# MoveBaseKnotCommand.Undo: pos = " << m_posBaseKnotOld << endl;
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_pBaseKnot->GetId( );
	}

private:
	BaseKnot * const m_pBaseKnot;
	MicroMeterPoint  m_posBaseKnotOld { NP_NULL };
};
