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
		ShapeId         const   idPipe, 
		MicroMeterPoint const & delta 
	)
      : MoveCommand( delta ),
		m_idPipe( idPipe )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_pPipe )
		{
			m_pPipe = nmwi.GetShapePtr<Pipe *>( m_idPipe );
			m_posStartKnotOld = m_pPipe->GetStartKnotPtr()->GetPosition();
			m_posEndKnotOld   = m_pPipe->GetEndKnotPtr  ()->GetPosition();
		}
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld + m_delta );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   + m_delta );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   );
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_idPipe;
	}

private:
	ShapeId   const m_idPipe;
	Pipe          * m_pPipe           { nullptr };
	MicroMeterPoint m_posStartKnotOld { NP_NULL };
	MicroMeterPoint m_posEndKnotOld   { NP_NULL };
};
