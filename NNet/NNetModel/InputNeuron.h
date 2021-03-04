// InputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include <sstream> 
#include "MoreTypes.h"
#include "Neuron.h"

class InputNeuron : public Neuron
{
public:

	InputNeuron( MicroMeterPoint const );

	virtual ~InputNeuron();

	virtual void CheckShape() const;

	virtual bool operator==( Shape const & ) const override;

	static unsigned long GetCounter  () { return m_counter; }
	static   void        ResetCounter() { m_counter = 0L; }
	virtual  void        IncCounter  () { ++ m_counter; }
	virtual  void        DecCounter  () { -- m_counter; }

	static bool TypeFits( ShapeType const type ) { return type.IsInputNeuronType( ); }

	virtual void DrawExterior  ( DrawContext const &, tHighlightType const ) const;
	virtual void DrawInterior  ( DrawContext const &, tHighlightType const ) const;

	virtual void Prepare( )
	{
		m_mVinputBuffer = m_mvFactor * m_timeSinceLastPulse.GetValue();
	}

	virtual bool CompStep( );
	virtual void DrawNeuronText( DrawContext const & ) const;
	virtual void Recalc( );   // Recalculate precalculated values

	fHertz const GetPulseFrequency( ) const { return m_pulseFrequency; }
	fHertz const SetPulseFrequency( fHertz const );

private:
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

	inline static unsigned long m_counter { 0L };
};	
