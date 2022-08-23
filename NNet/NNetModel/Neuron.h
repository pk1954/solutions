// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "BaseKnot.h"

import BoolOp;
import MoreTypes;
import tHighlight;
import Spike;

class DrawContext;
class IoLine;
class Pipe;

using std::unique_ptr;
using std::make_unique;

class Neuron : public BaseKnot
{
public:
	Neuron(MicroMeterPnt const &, NobType const = NobType::Value::neuron);
	Neuron(BaseKnot      const &, NobType const = NobType::Value::neuron);
	Neuron(Neuron const &);             // copy constructor

	Neuron & operator=(Neuron const &); // copy assignment operator

	~Neuron() override = default;

	void Prepare        ()                          override;
	void AppendMenuItems(AddMenuFunc const &) const override;

	static bool TypeFits(NobType const type) { return type.IsNeuronType(); }

	bool HasAxon() const { return HasOutgoing(); }

	void StopOnTrigger(tBoolOp const op) { ApplyOp(m_bStopOnTrigger, op); }

	void      SetDir(Radian const)  override { /* empty */ };
	void      ClearDynamicData()    override;
	bool      CompStep()            override;
	mV        GetNextOutput() const override;
	Radian    GetDir()        const override { return Radian::NULL_VAL(); };
	NobIoMode GetIoMode()     const override { return NobIoMode::internal; }

	void DrawExterior(DrawContext const &, tHighlight const) const override;
	void DrawInterior(DrawContext const &, tHighlight const) const override;

protected:

	void DisplayText(DrawContext const &, MicroMeterRect const &, wstring const &) const;

private:
	mutable bool m_bTriggered { false };

	bool       m_bStopOnTrigger { false };
	fMicroSecs m_usSpikeTime    { 0._MicroSecs };

	MicroMeterPnt getAxonHillockPos() const;

	void init(const Neuron &);
};

Neuron const * Cast2Neuron(Nob const *);
Neuron       * Cast2Neuron(Nob       *);
