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

	virtual void Prepare( NNetModel & );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;
	virtual void DrawInterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;

	Hertz GetPulseFrequency( ) const
	{
		return m_pulseFrequency;
	}

	void SetPulseFrequency( Hertz const );

private:
	void drawInputNeuron
	(  
		NNetModel     const &, 
		GraphicsInterface   &, 
		PixelCoordsFp const &,
		COLORREF      const,
		float         const
	) const;

	Hertz m_pulseFrequency;
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );
