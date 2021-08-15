// RotateNobCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "RotationCommand.h"
#include "Nob.h"

class RotateNobCommand : public RotationCommand
{
public:
	RotateNobCommand
	(
		Nob                 & nob,
		MicroMeterPnt const & umPntOld, 
		MicroMeterPnt const & umPntNew
	)
	  : m_nob(nob)
	{
		m_umPntPivot = nob.GetPos();
		calcRadDelta(umPntOld, umPntNew);
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.RotateNob(m_umPntPivot, m_radDelta);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.RotateNob(m_umPntPivot, -m_radDelta);
	}

	virtual NobId const GetAffectedNob() const { return m_nob.GetId(); }

private:
	Nob & m_nob;
};
