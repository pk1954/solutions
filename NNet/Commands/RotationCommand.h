// RotationCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class RotationCommand : public Command
{
public:

	virtual bool const CombineCommands(Command const & src) 
	{ 
		if (typeid(src) != typeid(*this))
			return false;
		RotationCommand const & srcCmd { static_cast<RotationCommand const &>(src) };
		m_radDelta += srcCmd.m_radDelta;
		return true; 
	}

protected:

	void calcRadDelta(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
	{
		Radian const radOld { Vector2Radian(umPntOld - m_umPntPivot) };
		Radian const radNew { Vector2Radian(umPntNew - m_umPntPivot) };
		m_radDelta = radNew - radOld;
	}

	Radian        m_radDelta;
	MicroMeterPnt m_umPntPivot;
};
