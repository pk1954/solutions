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
	MovePipeCommand( Pipe * const pPipe, MicroMeterPoint const & delta )
      : MoveCommand( delta ),
		m_pPipe          ( pPipe ),
		m_posStartKnotOld( pPipe->GetStartKnotPtr()->GetPosition() ),
		m_posEndKnotOld  ( pPipe->GetEndKnotPtr  ()->GetPosition())
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld + m_delta );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   + m_delta );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   );
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_pPipe->GetId( );
	}

private:
	Pipe          * const m_pPipe;
	MicroMeterPoint const m_posStartKnotOld;
	MicroMeterPoint const m_posEndKnotOld;
};
