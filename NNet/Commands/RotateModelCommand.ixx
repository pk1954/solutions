// RotateModelCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "UPNobList.h"
#include "Nob.h"
#include "BaseKnot.h"

export module RotateModelCommand;

import Types;
import RotationCommand;
import UPSensorList;

export class RotateModelCommand : public RotationCommand
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
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const& n) { return n.IsAnyNeuron(); }),
			umPntOld,
			umPntNew
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2All<BaseKnot>([this](BaseKnot& b) { DoRotate(b); });
		m_pNMWI->GetSensorList().Apply2All([this](Sensor* p) { DoRotateSensor(*p); });
	}

	void Undo() final
	{
		m_pNMWI->GetUPNobs().Apply2All<BaseKnot>([this](BaseKnot& b) { UndoRotate(b); });
		m_pNMWI->GetSensorList().Apply2All([this](Sensor* p) { UndoRotateSensor(*p); });
	}
};
