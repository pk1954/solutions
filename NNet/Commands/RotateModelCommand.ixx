// RotateModelCommand.ixx
//
// Commands

export module RotateModelCommand;

import Types;
import RotationCommand;
import NNetModel;

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
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const& n) { return n.IsPosNob(); }),
			umPntOld,
			umPntNew
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2All<PosNob>([this](PosNob& b) { DoRotate(b); });
		m_pNMWI->GetSensorList().Apply2All([this](Sensor* p) { DoRotateSensor(*p); });
	}

	void Undo() final
	{
		m_pNMWI->GetUPNobs().Apply2All<PosNob>([this](PosNob& b) { UndoRotate(b); });
		m_pNMWI->GetSensorList().Apply2All([this](Sensor* p) { UndoRotateSensor(*p); });
	}
};
