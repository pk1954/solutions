// OutputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "OutputNeuron.h"

OutputNeuron::OutputNeuron( NNetModel * pModel, MicroMeterPoint const upCenter )
	: Neuron( pModel, upCenter, tShapeType::outputNeuron )
{ 
	m_pNNetModel->IncNrOfOutputNeurons();
}

OutputNeuron::~OutputNeuron( )
{
	m_pNNetModel->DecNrOfOutputNeurons();
}

mV OutputNeuron::GetNextOutput( ) const 
{
	assert( false );  // Should never be called, as output neurons have no successor
	return 0._mV; 
};

void OutputNeuron::DrawExterior( PixelCoordsFp & coord ) const
{
	drawExterior( coord, 8 );
}

void OutputNeuron::DrawInterior( PixelCoordsFp & coord ) const
{ 
	drawInterior( coord, 8 );
}
