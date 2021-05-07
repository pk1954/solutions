// RotateCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Nob.h"

class RotateCommand : public Command
{
public:
	RotateCommand
	( 
		Nob                 & nob,
		MicroMeterPoint const & umPntOld, 
		MicroMeterPoint const & umPntNew
	)
	  : m_nob(nob)
	{
		m_umPntPivot = nob.GetPos();
		Radian const radOld { Vector2Radian(umPntOld - m_umPntPivot) };
		Radian const radNew { Vector2Radian(umPntNew - m_umPntPivot) };
		m_radDelta = radNew - radOld;
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_nob.RotateNob(m_umPntPivot, m_radDelta);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_nob.RotateNob(m_umPntPivot, -m_radDelta);
	}

	virtual NobId const GetMovedNob() const
	{
		return m_nob.GetId();
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

private:
	Nob         & m_nob;
	Radian          m_radDelta;
	MicroMeterPoint m_umPntPivot;
};
