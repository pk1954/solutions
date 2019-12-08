// OutputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Neuron.h"

class Pipeline;
class NNetModel;

class OutputNeuron : public Neuron
{
public:

	OutputNeuron( NNetModel *, MicroMeterPoint const );
	virtual ~OutputNeuron( );

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::outputNeuron;
	}

	virtual mV   GetNextOutput( ) const;
	virtual void DrawExterior ( PixelCoordsFp & ) const;
	virtual void DrawInterior ( PixelCoordsFp & ) const;
};	
