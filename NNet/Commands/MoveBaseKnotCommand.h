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
		ShapeId         const   id, 
		MicroMeterPoint const & delta 
	)
	  : MoveCommand( delta ),
		m_idBaseKnot( id )
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_pBaseKnot )
		{
			m_pBaseKnot      = nmwi.GetShapePtr<BaseKnot *>( m_idBaseKnot );
			m_posBaseKnotOld = m_pBaseKnot->GetPosition();
			//		wcout << Scanner::COMMENT_SYMBOL << L" MoveBaseKnotCommand: old pos = " << m_posBaseKnotOld << endl; 
		}
		m_pBaseKnot->SetPosition( m_posBaseKnotOld + m_delta );
//		wcout << Scanner::COMMENT_SYMBOL << L" MoveBaseKnotCommand.Do: pos = " << m_posBaseKnotOld + m_delta << endl; 
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld );
//		wcout << Scanner::COMMENT_SYMBOL << L" MoveBaseKnotCommand.Undo: pos = " << m_posBaseKnotOld << endl;
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_idBaseKnot;
	}

private:
	ShapeId    const m_idBaseKnot;
	BaseKnot       * m_pBaseKnot      { nullptr };
	MicroMeterPoint  m_posBaseKnotOld { NP_NULL };
};
