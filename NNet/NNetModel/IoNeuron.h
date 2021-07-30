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

	IoNeuron(Neuron const & src, NobType const type)
		: Neuron(src)
	{
		SetType(type);
	}

	void Check() const { Neuron::Check(); }

	virtual void RotateNob(MicroMeterPnt const &, Radian const);

	virtual Radian const GetDir() const;

	void SetDir      (Radian        const r) { m_radDirection = r;	}
	void SetDirVector(MicroMeterPnt const p) { SetDir(Vector2Radian(p)); }

	MicroMeterPnt    const GetScaledDirVector() const;
	MicroMeterPnt    const GetDirVector      () const;
	MicroMeterPosDir const GetRawPosDir      () const;
	MicroMeterPosDir const GetPosDir         () const;

	void UnlockDirection();

private:
	MicroMeterPnt const determineVector() const;

	Radian m_radDirection { Radian::NULL_VAL() };
};
