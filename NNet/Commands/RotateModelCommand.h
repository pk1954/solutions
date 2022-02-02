// RotateModelCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "RotationCommand.h"

class RotateModelCommand : public RotationCommand
{
public:
	RotateModelCommand
	(
		MicroMeterPnt const & umPntOld, 
		MicroMeterPnt const & umPntNew
	)
	{
		m_umPntPivot = m_pNMWI->GetUPNobs().CenterOfGravity
		(
			[&](Nob const & nob){ return nob.IsAnyNeuron(); }
		);
		calcRadDelta(umPntOld, umPntNew);
	}

	void Do() final 
	{ 
		m_pNMWI->GetUPNobs().Apply2All<BaseKnot>([&](BaseKnot & b) { b.RotateNob(m_umPntPivot, m_radDelta); });
	}

	void Undo() final 
	{ 
		m_pNMWI->GetUPNobs().Apply2All<BaseKnot>([&](BaseKnot & b) { b.RotateNob(m_umPntPivot, -m_radDelta); });
	}
};
