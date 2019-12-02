// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "win32_graphicsInterface.h"
#include "PixelCoordsFp.h"
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
	float fillLevel { m_timeSinceLastPulse.GetValue() / m_pulseDuration.GetValue() };
	m_mVinputBuffer = mV( model.GetParameterValue( tParameter::peakVoltage ) * fillLevel );
}

void InputNeuron::Step( NNetModel const & model )
{
	if ( m_timeSinceLastPulse >= m_pulseDuration )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;   
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
	MicroMeterPoint  const axonVector { pAxon->GetVector( model ) };
	MicroMeter       const umHypot    { Hypot( axonVector ) };
	MicroMeterPoint  const umExtVector{ axonVector * (GetExtension() / umHypot) };
	MicroMeterPoint  const umCenter   { GetPosition() };
	MicroMeterPoint  const umStartPnt { umCenter + umExtVector * fReductionFactor };
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
	drawInputNeuron( model, coord, GetFrameColor( ), 1.0f );
}

void InputNeuron::DrawInterior( NNetModel const & model, PixelCoordsFp & coord ) const
{ 
	drawInputNeuron( model, coord, GetInteriorColor( model ), NEURON_INTERIOR );
}

void InputNeuron::DrawNeuronText( NNetModel const & model, PixelCoordsFp & coord )
{ 
	static COLORREF const color { RGB( 0, 255, 0 ) };

	fPixelPoint const fPos   { coord.convert2fPixelPos( GetPosition() ) }; 
	fPIXEL      const fExt   { coord.convert2fPixel( GetExtension() ) };
	PixelPoint  const pixPos { convert2PixelPoint( fPos ) };
	PIXEL       const pixExt { PIXEL(static_cast<long>(fExt.GetValue())) };
	PixelRect   const pixRect
	{
		pixPos.GetX() - pixExt,  // left
		pixPos.GetY() - pixExt,  // top
		pixPos.GetX() + pixExt,  // right
		pixPos.GetY() + pixExt   // bottom
	};
	m_wBuffer.clear( );
	m_wBuffer.str( std::wstring() );
	m_wBuffer << GetPulseFrequency().GetValue() << L" " << model.GetParameterUnit( tParameter::pulseRate );
	if ( pixRect.Includes( m_pGraphics->CalcGraphicsRect( m_wBuffer.str( ) ).GetSize() ) )
	{
		m_pGraphics->DisplayGraphicsText( pixRect, m_wBuffer.str( ), DT_CENTER|DT_VCENTER, color );
		return;
	}
	m_wBuffer.clear( );
	m_wBuffer.str( std::wstring() );
	m_wBuffer << GetPulseFrequency().GetValue();
	if ( pixRect.Includes( m_pGraphics->CalcGraphicsRect( m_wBuffer.str( ) ).GetSize() ) )
	{
		m_pGraphics->DisplayGraphicsText( pixRect, m_wBuffer.str( ), DT_CENTER|DT_VCENTER, color );
	}
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
