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

	virtual void Prepare( );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const { return 0._mV; };
	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;
};	

OutputNeuron const * Cast2OutputNeuron( Shape const * );
OutputNeuron       * Cast2OutputNeuron( Shape       * );
