// MovePipeCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "MoveCommand.h"
#include "Pipe.h"

class MovePipeCommand : public MoveCommand
{
public:
	MovePipeCommand
	( 
		NNetModelWriterInterface & model, 
		ShapeId            const   idPipe, 
		MicroMeterPoint    const & delta 
	)
      : MoveCommand( delta ),
		m_pPipe( model.GetShapePtr<Pipe *>( idPipe ) )
	{ 
		m_posStartKnotOld = m_pPipe->GetStartKnotPtr()->GetPosition();
		m_posEndKnotOld   = m_pPipe->GetEndKnotPtr  ()->GetPosition();
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld + m_delta );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   + m_delta );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   );
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_pPipe->GetId( );
	}

private:
	Pipe    * const m_pPipe;
	MicroMeterPoint m_posStartKnotOld { NP_NULL };
	MicroMeterPoint m_posEndKnotOld   { NP_NULL };
};
