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
	static bool const TypeFits(NobType const type) { return type.IsIoNeuronType(); }

	IoNeuron(MicroMeterPnt const & upCenter, NobType const type)
		: Neuron(upCenter, type)
	{}

	IoNeuron(BaseKnot const & src, NobType const type)
		: Neuron(src.GetPos(), type)
	{
		SetId(src.GetId());
	}

	void Check() const { Neuron::Check(); }

	virtual void RotateNob(MicroMeterPnt const &, Radian const);

	virtual Radian const GetDir() const;
	virtual void         SetDir(Radian const);
	                                // IoNeurons can be locked or unlocked
	void       LockDirection();     // locked: m_radDirection is not null, it is displayed with this direction
	void       UnlockDirection();   // unlocked: m_radDirection is null, displaed direction
	bool const IsDirLocked() const; //           is computed depending on connected pipe(s)

	MicroMeterPnt    const GetScaledDirVector() const;
	MicroMeterPnt    const GetDirVector      () const;
	MicroMeterPosDir const GetRawPosDir      () const;
	MicroMeterPosDir const GetPosDir         () const;


private:
	MicroMeterPnt const determineVector() const;

	Radian m_radDirection { Radian::NULL_VAL() };
};
