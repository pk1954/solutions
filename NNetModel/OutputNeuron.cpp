// OutputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "OutputNeuron.h"

OutputNeuron::OutputNeuron( MicroMeterPoint const upCenter )
	: Neuron( upCenter, tShapeType::outputNeuron )
{ 
}

mV OutputNeuron::GetNextOutput( NNetModel const & ) const 
{
	assert( false );  // Should never be called, as output neurons have no successor
	return 0._mV; 
};

void OutputNeuron::DrawExterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	drawExterior( coord, 8 );
}

void OutputNeuron::DrawInterior( NNetModel const & model, PixelCoordsFp & coord ) const
{ 
	drawInterior( model, coord, 8 );
}

OutputNeuron const * Cast2OutputNeuron( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::outputNeuron );
	return static_cast<OutputNeuron const *>(shape);
}

OutputNeuron * Cast2OutputNeuron( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::outputNeuron );
	return static_cast<OutputNeuron *>(shape);
}
