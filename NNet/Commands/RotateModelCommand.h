// RotateModelCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class RotateModelCommand : public Command
{
public:
	RotateModelCommand
	(
		NNetModelReaderInterface & nmri,
		MicroMeterPnt      const & umPntOld, 
		MicroMeterPnt      const & umPntNew
	)
	{
		m_umPntPivot = nmri.CenterOfGravity();
		Radian const radOld { Vector2Radian(umPntOld - m_umPntPivot) };
		Radian const radNew { Vector2Radian(umPntNew - m_umPntPivot) };
		m_radDelta = radNew - radOld;
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
	    nmwi.GetUPNobs().Rotate(m_umPntPivot, m_radDelta);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		nmwi.GetUPNobs().Rotate(m_umPntPivot, -m_radDelta);
	}

	virtual bool const CombineCommands(Command const & src) 
	{ 
		if (typeid(src) != typeid(*this))
			return false;
		RotateModelCommand const & srcCmd { static_cast<RotateModelCommand const &>(src) };
		if (m_umPntPivot != srcCmd.m_umPntPivot)
			return false;
		m_radDelta += srcCmd.m_radDelta;
		return true; 
	};

private:
	Radian        m_radDelta;
	MicroMeterPnt m_umPntPivot;
};
