// RotateModelCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
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
		SetPivotPnt
		(
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const &n){ return n.IsAnyNeuron(); }),
			umPntOld, 
			umPntNew
		);
	}

	void Do() final 
	{ 
		m_pNMWI->GetUPNobs().Apply2All<BaseKnot>([this](BaseKnot &b) { DoRotate(b); });
		m_pNMWI->GetMonitorData().Apply2AllSignals([this](Signal & s){ DoRotateSensor(s); });
	}

	void Undo() final 
	{ 
		m_pNMWI->GetUPNobs().Apply2All<BaseKnot>([this](BaseKnot &b) { UndoRotate(b); });
		m_pNMWI->GetMonitorData().Apply2AllSignals([this](Signal & s){ UndoRotateSensor(s); });
	}
};
