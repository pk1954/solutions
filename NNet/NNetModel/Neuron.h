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
	Neuron(MicroMeterPnt const &, IoNeuron const &, IoNeuron const &);
	Neuron(Neuron const & );   // copy constructor

	virtual void Check() const;

	Neuron & operator=( Neuron const & ); // copy assignment operator

	virtual ~Neuron();

	virtual bool operator==( Nob const & ) const override;

	static bool    const TypeFits(NobType const type) { return type.IsNeuronType(); }
	static NobType const GetNobType() { return NobType::Value::neuron; }

	bool       const HasAxon         () const { return m_connections.HasOutgoing(); }
	bool       const HasTriggerSound () const { return m_triggerSound.m_bOn; }
	SoundDescr const GetTriggerSound () const { return m_triggerSound; }

	SoundDescr const SetTriggerSound( SoundDescr const & );

	fMicroSecs const PulseWidth   () const;
	fMicroSecs const RefractPeriod() const;
	mV         const Threshold    () const;
	mV         const PeakVoltage  () const;

	void StopOnTrigger(tBoolOp const op) { ApplyOp( m_bStopOnTrigger, op ); }

	virtual void         SetDir(Radian const r) { };
	virtual void         DrawExterior  ( DrawContext const &, tHighlight const) const;
	virtual void         DrawInterior  ( DrawContext const &, tHighlight const) const;
	virtual void         DrawNeuronText( DrawContext const & ) const;
	virtual void         Recalc();
	virtual void         Clear();
	virtual bool   const CompStep();
	virtual mV     const GetNextOutput() const;
	virtual Radian const GetDir()        const { return Radian::NULL_VAL(); };

	virtual NobIoMode const GetIoMode() const { return NobIoMode::internal; }

	static void SetSound(Sound * const pSound) { m_pSound = pSound; }

protected:
	fMicroSecs m_timeSinceLastPulse { 0._MicroSecs };
	bool       m_bStopOnTrigger     { false };

	mV waveFunction( fMicroSecs const ) const;

	void const DisplayText( DrawContext const &, MicroMeterRect const &, wstring const ) const;

private:
	mutable bool m_bTriggered { false };

	float m_factorW; // Parameter of wave function
	float m_factorU; // Parameter of wave function

	SoundDescr m_triggerSound {};

	PTP_WORK  m_pTpWork { nullptr };  // Thread poolworker thread

	MicroMeterPnt getAxonHillockPos() const;

	inline static Sound * m_pSound  { nullptr };

	void init( const Neuron & );

	friend static void CALLBACK BeepFunc( PTP_CALLBACK_INSTANCE, PVOID,	PTP_WORK );
};

Neuron const * Cast2Neuron( Nob const * );
Neuron       * Cast2Neuron( Nob       * );
