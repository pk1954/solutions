// InputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "IoNeuron.h"

class Nob;
class BaseKnot;
class DrawContext;

class InputNeuron : public IoNeuron
{
public:

	InputNeuron(MicroMeterPnt const &);
	InputNeuron(BaseKnot      const &);

	virtual ~InputNeuron();

	virtual void Check() const;

	virtual bool operator==(Nob const &) const override;

	static bool const TypeFits(NobType const type) { return type.IsInputNeuronType(); }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const;

	virtual NobIoMode const GetIoMode() const { return NobIoMode::input; }

	virtual void Prepare()
	{
		m_mVinputBuffer = m_mvFactor * m_timeSinceLastPulse.GetValue();
	}

	virtual bool const CompStep();
	virtual void       DrawNeuronText(DrawContext const &) const;
	virtual void       Recalc();   // Recalculate precalculated values

	fHertz const GetPulseFrequency() const { return m_pulseFrequency; }
	fHertz const SetPulseFrequency(fHertz const);

	virtual bool const Includes(MicroMeterPnt const &) const;

private:

	MicroMeterPnt const getOffset() const;
	MicroMeterPnt const getCenter() const;

	void drawSocket
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;

	mV         m_mvFactor;       // precomputed value for optimization
	fHertz     m_pulseFrequency; // pulse frequency and pulse duration depend on each other
	fMicroSecs m_pulseDuration;  // in principle one variable would be enough, but to avoid 
                                 // floating point rounding effects, both are stored
};	
