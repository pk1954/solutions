// RotateSelectionCommand.ixx
//
// Commands

export module RotateSelectionCommand;

import Types;
import RotationCommand;
import NNetModel;

export class RotateSelectionCommand : public RotationCommand
{
public:
	RotateSelectionCommand
	(
		MicroMeterPnt const& umPntOld,
		MicroMeterPnt const& umPntNew
	)
	{
		SetPivotPnt
		(
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const& n) { return n.IsSelected() && n.IsPosNob(); }),
			umPntOld,
			umPntNew
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>([this](PosNob& b) { DoRotate(b); });
	}

	void Undo() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>([this](PosNob& b) { UndoRotate(b); });
	}
};
