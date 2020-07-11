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
	virtual ~InputNeuron( );

	virtual bool IsEqual( InputNeuron const & other ) const
	{
		if ( ! Neuron::IsEqual( other ) )
			return false;
		if ( m_mvFactor != other.m_mvFactor )
			return false;
		if ( m_pulseFrequency != other.m_pulseFrequency )
			return false;
		if ( m_pulseDuration != other.m_pulseDuration )
			return false;
		return true;
	}

	static unsigned long GetCounter( ) { return m_counter; }

	static bool TypeFits( ShapeType const type ) { return type.IsInputNeuronType( ); }

	virtual void Prepare( )
	{
		m_mVinputBuffer = m_mvFactor * m_timeSinceLastPulse.GetValue();
	}

	virtual bool CompStep( )
	{
		bool bTrigger { m_timeSinceLastPulse >= m_pulseDuration };
		if ( bTrigger )
		{
			m_timeSinceLastPulse = 0._MicroSecs;   
		}
		else
		{
			m_timeSinceLastPulse += m_pParameters->GetTimeResolution( );
		}
		return m_bStopOnTrigger && bTrigger;
	}

	virtual void DrawExterior  ( DrawContext const &, tHighlightType const = tHighlightType::normal ) const;
	virtual void DrawInterior  ( DrawContext const & ) const;
	virtual void DrawNeuronText( DrawContext const & ) const;
	virtual void Recalc( );   // Recalculate precalculated values

	virtual InputNeuron * Clone( ) const { return new InputNeuron( * this ); };

	fHertz const GetPulseFrequency( ) const { return m_pulseFrequency; }

	fHertz const SetPulseFrequency( fHertz const );

private:
	void drawInputNeuron( DrawContext const &, D2D1::ColorF const, float const ) const;

	mV         m_mvFactor;       // precomputed value for optimization
	fHertz     m_pulseFrequency; // pulse frequency and pulse duration depend on each other
	fMicroSecs m_pulseDuration;  // in principle one variable would be enough, but to avoid 
	                             // floating point rounding effects, both are stored

	inline static unsigned long m_counter { 0L };
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );
