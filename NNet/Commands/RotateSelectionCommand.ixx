// RotateSelectionCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"

export module RotateSelectionCommand;

import MoreTypes;
import RotationCommand;

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
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const& n) { return n.IsSelected() && n.IsAnyNeuron(); }),
			umPntOld,
			umPntNew
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<BaseKnot>([this](BaseKnot& b) { DoRotate(b); });
	}

	void Undo() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<BaseKnot>([this](BaseKnot& b) { UndoRotate(b); });
	}
};
