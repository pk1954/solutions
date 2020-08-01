// MovePipeCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "Pipe.h"

class MovePipeCommand : public Command
{
public:
	MovePipeCommand( Pipe * const pPipe, MicroMeterPoint const & delta )
		: m_pPipe          ( pPipe ),
		m_delta          ( delta ),
		m_posStartKnotOld( pPipe->GetStartKnotPtr()->GetPosition() ),
		m_posEndKnotOld  ( pPipe->GetEndKnotPtr  ()->GetPosition())
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld + m_delta );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   + m_delta  );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld );
		m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   );
	}

private:
	Pipe          * const m_pPipe;
	MicroMeterPoint const m_delta;
	MicroMeterPoint const m_posStartKnotOld;
	MicroMeterPoint const m_posEndKnotOld;
};
