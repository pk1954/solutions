// OutputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Neuron.h"

using std::chrono::microseconds;

class Pipeline;

class OutputNeuron : public Neuron
{
public:

	OutputNeuron( MicroMeterPoint const );

	virtual void Prepare( NNetModel & );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const { return 0._mV; };
	virtual void DrawExterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;
	virtual void DrawInterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;
};	

OutputNeuron const * Cast2OutputNeuron( Shape const * );
OutputNeuron       * Cast2OutputNeuron( Shape       * );
