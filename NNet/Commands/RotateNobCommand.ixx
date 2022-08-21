// RotateNobCommand.ixx
//
// Commands

module;

#include "Nob.h"
#include "NobId.h"

export module RotateNobCommand;

import MoreTypes;
import RotationCommand;

export class RotateNobCommand : public RotationCommand
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
		SetPivotPnt(nob.GetPos(), umPntOld, umPntNew);
	}

	void Do()   final { DoRotate(m_nob); }
	void Undo() final { UndoRotate(m_nob); }

	NobId GetAffectedNob() const final { return m_nob.GetId(); }

private:
	Nob & m_nob;
};
