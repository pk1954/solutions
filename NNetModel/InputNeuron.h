// InputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include <sstream> 
#include "MoreTypes.h"
#include "Neuron.h"

class Pipeline;
class NNetModel;

class InputNeuron : public Neuron
{
public:

	InputNeuron( NNetModel *, MicroMeterPoint const );
	virtual ~InputNeuron( );

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::inputNeuron;
	}

	virtual void Prepare( );
	virtual void Step   ( );

	virtual void DrawExterior( PixelCoordsFp & ) const;
	virtual void DrawInterior( PixelCoordsFp & ) const;
	virtual void DrawText    ( PixelCoordsFp & ) const;

	fHertz GetPulseFrequency( ) const {	return m_pulseFrequency; }

	void SetPulseFrequency( fHertz const );

private:
	void drawInputNeuron( PixelCoordsFp const &, COLORREF const, float const ) const;

	fHertz    m_pulseFrequency; // pulse frequency and pulse duration depend on each other
	MicroSecs m_pulseDuration;  // in principle one variable would be enough, but to avoid 
	                            // floating point rounding effects, both are stored
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );
