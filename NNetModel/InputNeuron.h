// InputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Neuron.h"

using std::chrono::microseconds;

class Pipeline;

class InputNeuron : public Neuron
{
public:

	InputNeuron( MicroMeterPoint const );

	virtual void Prepare( );
	virtual void Step( );

	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;

	fHertz GetPulseFrequency( ) const
	{
		return m_pulseFrequency;
	}

	void SetPulseFrequency( fHertz const );

private:
	void drawInputNeuron( COLORREF const, float const ) const;

	fHertz       m_pulseFrequency; // pulse frequency and pulse duration depend on each other
	microseconds m_pulseDuration;  // in principle one variable would be enough, but to avoid 
	                               // floating point rounding effects, both are stored
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );
