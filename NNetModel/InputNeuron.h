// InputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include <sstream> 
#include "MoreTypes.h"
#include "Neuron.h"

using std::chrono::microseconds;

class Pipeline;

class InputNeuron : public Neuron
{
public:

	InputNeuron( MicroMeterPoint const );

	virtual void Prepare( NNetModel const & );
	virtual void Step   ( NNetModel const & );

	virtual void DrawExterior( NNetModel const &, PixelCoordsFp & ) const;
	virtual void DrawInterior( NNetModel const &, PixelCoordsFp & ) const;

	void DrawNeuronText( NNetModel const &, PixelCoordsFp & );

	fHertz GetPulseFrequency( ) const
	{
		return m_pulseFrequency;
	}

	void SetPulseFrequency( fHertz const );

private:
	void drawInputNeuron( NNetModel const &, PixelCoordsFp const &,	COLORREF const, float const ) const;

	std::wostringstream m_wBuffer;

	fHertz    m_pulseFrequency; // pulse frequency and pulse duration depend on each other
	MicroSecs m_pulseDuration;  // in principle one variable would be enough, but to avoid 
	                            // floating point rounding effects, both are stored
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );
