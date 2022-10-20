// Neuron.ixx
//
// NNetModel

module;

#include <string>
#include <chrono>

export module NNetModel:Neuron;

import BoolOp;
import Types;
import DrawContext;
import :tHighlight;
import :Spike;
import :NobType;
import :BaseKnot;

using std::wstring;
using std::unique_ptr;
using std::make_unique;

export class Neuron : public BaseKnot
{
public:
	Neuron(MicroMeterPnt const &);
	Neuron(Neuron        const &);             // copy constructor

	Neuron& operator=(Neuron const&); // copy assignment operator

	~Neuron() override = default;

	void CollectInput()  final;
	bool CompStep()      final;

	void AppendMenuItems(AddMenuFunc const &) const override;

	static bool TypeFits(NobType const type) { return type.IsNeuronType(); }

	bool HasAxon() const { return HasOutgoing(); }

	void StopOnTrigger(tBoolOp const op) { ApplyOp(m_bStopOnTrigger, op); }

	void      SetDir(Radian const)  final { /* empty */ };
	void      ClearDynamicData()    final;
	mV        GetNextOutput() const final;
	Radian    GetDir()        const final { return Radian::NULL_VAL(); };
	NobIoMode GetIoMode()     const final { return NobIoMode::internal; }

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

protected:

	void DisplayText(DrawContext const&, MicroMeterRect const&, wstring const&) const;

private:
	mutable bool m_bTriggered{ false };

	bool       m_bStopOnTrigger{ false };
	fMicroSecs m_usSpikeTime{ 0._MicroSecs };

	MicroMeterPnt getAxonHillockPos() const;

	void init(const Neuron&);
};

export Neuron const * Cast2Neuron(Nob const *);
export Neuron       * Cast2Neuron(Nob       *);
