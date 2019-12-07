// OutputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Neuron.h"

class Pipeline;

class OutputNeuron : public Neuron
{
public:

	OutputNeuron( MicroMeterPoint const );

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::outputNeuron;
	}

	virtual mV   GetNextOutput( NNetModel const & ) const;
	virtual void DrawExterior ( NNetModel const &, PixelCoordsFp & ) const;
	virtual void DrawInterior ( NNetModel const &, PixelCoordsFp & ) const;
};	
