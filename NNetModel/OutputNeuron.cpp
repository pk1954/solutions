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

void OutputNeuron::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pModel->GetPipeline( idPipeline )->GetNextOutput();
	if (m_mVinputBuffer > PEAK_VOLTAGE)
		m_mVinputBuffer = PEAK_VOLTAGE;
}

void OutputNeuron::Step( )
{
}

void OutputNeuron::DrawExterior( ) const
{
	drawExterior( 8 );
}

void OutputNeuron::DrawInterior( ) const
{ 
	drawInterior( 8 );
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
