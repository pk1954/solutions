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

	void Trigger( );

	virtual void Prepare( );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;

	Hertz GetPulseFrequency( ) const
	{
		return m_pulseFrequency;
	}

	void SetPulseFrequency( Hertz const );

private:
	void drawInputNeuron( COLORREF const, float const ) const;

	Hertz m_pulseFrequency;
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );
