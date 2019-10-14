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
	: BaseKnot( tShapeType::outputNeuron, upCenter, 50.0_MicroMeter )
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

PERCENT OutputNeuron::GetFillLevel( ) const
{
	return PERCENT( CastToShort( (m_mVinputBuffer * 100) / PEAK_VOLTAGE ) );
}

void OutputNeuron::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	COLORREF const colorFrame = IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 );
	Graphics.DrawPolygon
	( 
		8,
		coord.convert2fPixelPos( GetPosition() ), 
		IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ), 
		coord.convert2fPixel( GetExtension() )
	);
}

void OutputNeuron::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{ 
	PERCENT  const fillLevel = GetFillLevel();
	int      const colElem   = ( 255 * fillLevel.GetValue() ) / 100;
	COLORREF const color     = RGB( colElem, 0, 0 );
	Graphics.DrawPolygon
	( 
		8,
		coord.convert2fPixelPos( GetPosition() ), 
		color, 
		coord.convert2fPixel( GetExtension() * 0.8f )
	);
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
