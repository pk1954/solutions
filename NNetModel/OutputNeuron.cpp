// OutputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "OutputNeuron.h"

using namespace std::chrono;

OutputNeuron::OutputNeuron( MicroMeterPoint const upCenter )
	: Neuron( upCenter, tShapeType::outputNeuron )
{ 
}

void OutputNeuron::Prepare( NNetModel & model )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incomming )
		m_mVinputBuffer += model.GetPipeline( idPipeline )->GetNextOutput();
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );
}

void OutputNeuron::Step( )
{
}

void OutputNeuron::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	drawExterior( model, Graphics, coord, 8 );
}

void OutputNeuron::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{ 
	drawInterior( model, Graphics, coord, 8 );
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
