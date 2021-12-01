// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "SoundInterface.h"
#include "tHighlightType.h"
#include "BaseKnot.h"

class DrawContext;
class IoNeuron;
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

	bool operator==(Nob const &) const override;

	void AppendMenuItems(AddMenuFunc const &) const override;

	static bool TypeFits(NobType const type) { return type.IsNeuronType(); }

	bool       HasAxon         () const { return HasOutgoing(); }
	bool       HasTriggerSound () const { return m_triggerSound.m_bOn; }
	SoundDescr GetTriggerSound () const { return m_triggerSound; }

	SoundDescr SetTriggerSound(SoundDescr const &);

	void StopOnTrigger(tBoolOp const op) { ApplyOp(m_bStopOnTrigger, op); }

	void      SetDir(Radian const)  override { };
	void      DrawExterior  (DrawContext const &, tHighlight const) const override;
	void      DrawInterior  (DrawContext const &, tHighlight const) const override;
	void      ClearDynamicData() override;
	bool      CompStep() override;
	mV        GetNextOutput() const override;
	Radian    GetDir()        const override { return Radian::NULL_VAL(); };
	NobIoMode GetIoMode()     const override { return NobIoMode::internal; }

	virtual void DrawNeuronText(DrawContext const &) const;

	void Recalc() final;
	void SetDirVector(MicroMeterPnt const p) { SetDir(Vector2Radian(p)); }

	static void SetSound(Sound * const pSound) { m_pSound = pSound; }

protected:

	void DisplayText(DrawContext const &, MicroMeterRect const &, wstring const &) const;

private:
	mutable bool m_bTriggered { false };

	SoundDescr m_triggerSound     {};
	float      m_factorW          { 0.0f }; // Parameter of wave function
	float      m_factorU          { 0.0f }; // Parameter of wave function
	bool       m_bStopOnTrigger   { false };
	PTP_WORK   m_pTpWork          { nullptr };  // Thread poolworker thread
	fMicroSecs m_usSinceLastPulse { 0._MicroSecs };

	inline static Sound * m_pSound  { nullptr };

	mV waveFunction(fMicroSecs const) const;

	MicroMeterPnt getAxonHillockPos() const;

	void init(const Neuron &);

	friend static void CALLBACK BeepFunc(PTP_CALLBACK_INSTANCE, PVOID,	PTP_WORK);
};

Neuron const * Cast2Neuron(Nob const *);
Neuron       * Cast2Neuron(Nob       *);
