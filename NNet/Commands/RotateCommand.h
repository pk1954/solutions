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
		Nob                   & nob,
		MicroMeterPnt const & umPntOld, 
		MicroMeterPnt const & umPntNew
	)
	  : m_nob(nob)
	{
		m_umPntPivot = nob.GetPos();
		m_radDelta = Vector2Radian(umPntNew - m_umPntPivot) - Vector2Radian(umPntOld - m_umPntPivot);
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.RotateNob(m_umPntPivot, m_radDelta);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
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

	virtual bool const CombineCommands(Command const & src) 
	{ 
		if (typeid(src) != typeid(*this))
			return false;
		RotateCommand const & srcCmd { static_cast<RotateCommand const &>(src) };
		if (GetMovedNob() != srcCmd.GetMovedNob())
			return false;
		if (m_umPntPivot != srcCmd.m_umPntPivot)
			return false;
		m_radDelta += srcCmd.m_radDelta;
		return true; 
	};

private:
	Nob           & m_nob;
	Radian          m_radDelta;
	MicroMeterPnt m_umPntPivot;
};
