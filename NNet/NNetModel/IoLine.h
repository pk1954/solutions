// IoLine.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "MicroMeterPosDir.h"
#include "NNetParameters.h"
#include "BaseKnot.h"

import MoreTypes;

class IoLine : public BaseKnot
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoLineType(); }

	IoLine(MicroMeterPnt const & upCenter, NobType const type)
		: BaseKnot(upCenter, type, NEURON_RADIUS)
	{}

	IoLine(BaseKnot const & src, NobType const type)
		: BaseKnot(src)
	{
		SetId(src.GetId());
		SetType(type);
		SetExtension(NEURON_RADIUS);
	}

	bool CompStep() final { return false; }

	MicroMeterPosDir GetPosDir()    const override;
	Radian           GetDir()       const override;
	void             SetDir(Radian const) override;

	void SetDirVector(MicroMeterPnt const p) { SetDir(Vector2Radian(p)); }

	                          // IoLines can be locked or unlocked
	void LockDirection();     // locked: m_radDirection is not null, it is displayed with this direction
	void UnlockDirection();   // unlocked: m_radDirection is null, displayed direction
	bool IsDirLocked() const; //           is computed depending on connected pipe(s)

	MicroMeterPnt    GetScaledDirVector() const;
	MicroMeterPnt    GetDirVector      () const;
	MicroMeterPosDir GetRawPosDir      () const;

private:
	MicroMeterPnt determineVector() const;

	Radian m_radDirection { Radian::NULL_VAL() };
};
