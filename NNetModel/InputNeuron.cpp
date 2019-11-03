// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "InputNeuron.h"

using namespace std::chrono;

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
  : Neuron( upCenter, tShapeType::inputNeuron ),
	m_pulseFrequency( STD_PULSE_FREQ ),
	m_pulseDuration( PulseDuration( STD_PULSE_FREQ ) )
{ 
}

void InputNeuron::SetPulseFrequency( fHertz const freq )
{
	m_pulseFrequency = freq;
	m_pulseDuration  = PulseDuration( m_pulseFrequency );
}

void InputNeuron::Prepare( NNetModel const & model )
{
	float fillLevel = static_cast<float>(m_timeSinceLastPulse.count()) / static_cast<float>(m_pulseDuration.count());
	m_mVinputBuffer = model.GetPeakVoltage() * fillLevel;
}

void InputNeuron::Step( NNetModel const & model )
{
	if ( m_timeSinceLastPulse >= m_pulseDuration )  
	{
		m_timeSinceLastPulse = 0ms;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}
}

void InputNeuron::drawInputNeuron
( 
	NNetModel     const & model,
	PixelCoordsFp const & coord,
	COLORREF      const   color,
	float         const   fReductionFactor
) const
{
	ShapeId          const idAxon     { * m_outgoing.begin() };
	Pipeline const * const pAxon      { model.GetConstPipeline( idAxon ) };
	MicroMeterPoint  const umStart    { pAxon->GetStartPoint( model ) };
	MicroMeterPoint  const umEnd      { pAxon->GetEndPoint  ( model ) };
	MicroMeterPoint  const umVector   { umEnd - umStart };
	MicroMeter       const umHypot    { Hypot( umVector ) };
	MicroMeterPoint  const umExtVector{ umVector * (GetExtension() / umHypot) };
	MicroMeterPoint  const umCenter   { GetPosition() };
	MicroMeterPoint  const umStartPnt { umCenter + umExtVector  * fReductionFactor };
	MicroMeterPoint  const umEndPnt   { umCenter - umExtVector };
	fPixelPoint      const fStartPoint{ coord.convert2fPixelPos( umStartPnt ) };
	fPixelPoint      const fEndPoint  { coord.convert2fPixelPos( umEndPnt   ) };
	fPIXEL           const fPixWidth  { coord.convert2fPixel( GetExtension() * fReductionFactor ) };

	m_pGraphics->StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
	m_pGraphics->AddPipelinePoint( fEndPoint, color );
	m_pGraphics->RenderPipeline( );
}

void InputNeuron::DrawExterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	drawInputNeuron( model, coord, GetFrameColor( ),1.0f );
}

void InputNeuron::DrawInterior( NNetModel const & model, PixelCoordsFp & coord ) const
{ 
	drawInputNeuron( model, coord, GetInteriorColor( model ), NEURON_INTERIOR );
}

InputNeuron const * Cast2InputNeuron( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<InputNeuron const *>(shape);
}

InputNeuron * Cast2InputNeuron( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<InputNeuron *>(shape);
}
