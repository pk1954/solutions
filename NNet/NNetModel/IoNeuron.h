// IoNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "Neuron.h"

using std::wcout;
using std::endl;

class IoNeuron : public Neuron
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoNeuronType(); }

	IoNeuron(MicroMeterPnt const & upCenter, NobType const type)
		: Neuron(upCenter, type)
	{}

	IoNeuron(BaseKnot const & src, NobType const type)
		: Neuron(src.GetPos(), type)
	{
		SetId(src.GetId());
	}

	void RotateNob(MicroMeterPnt const &, Radian const) final;

	MicroMeterPosDir GetPosDir()    const override;
	Radian           GetDir()       const override;
	void             SetDir(Radian const) override;

	                          // IoNeurons can be locked or unlocked
	void LockDirection();     // locked: m_radDirection is not null, it is displayed with this direction
	void UnlockDirection();   // unlocked: m_radDirection is null, displaed direction
	bool IsDirLocked() const; //           is computed depending on connected pipe(s)

	MicroMeterPnt    GetScaledDirVector() const;
	MicroMeterPnt    GetDirVector      () const;
	MicroMeterPosDir GetRawPosDir      () const;

private:
	MicroMeterPnt determineVector() const;

	Radian m_radDirection { Radian::NULL_VAL() };
};
