// Pipeline.cpp 
//
// NNetModel

#include "stdafx.h"
#include <cmath>
#include "assert.h"
#include "Geometry.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Pipeline.h"

Pipeline::Pipeline( meterPerSec const impulseSpeed )
  :	Shape( tShapeType::pipeline ),
	m_initialized ( false ),
	m_idKnotStart ( NO_SHAPE ),
	m_idKnotEnd   ( NO_SHAPE ),
	m_width       ( 10.0_MicroMeter ),
	m_potential   ( ),
	m_impulseSpeed( impulseSpeed )
{
}

void Pipeline::Resize( NNetModel & model )
{
	if ( IsDefined(m_idKnotStart)  && IsDefined(m_idKnotEnd) )
	{
		MicroMeter const segmentLength  = CoveredDistance( m_impulseSpeed, TIME_RESOLUTION );
		BaseKnot * const m_pKnotStart   = model.GetBaseKnot( m_idKnotStart );   
		BaseKnot * const m_pKnotEnd     = model.GetBaseKnot( m_idKnotEnd   );   
		MicroMeter const pipelineLength = Distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() );
		unsigned int     iNrOfSegments  = CastToUnsignedInt(round(pipelineLength / segmentLength));

		if ( iNrOfSegments == 0 )
			iNrOfSegments = 1;

		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
		m_initialized = true;
	}
}

void Pipeline::initialize( NNetModel & model )
{
	if ( ! m_initialized  )
		Resize( model );
}

void Pipeline::SetStartKnot( NNetModel & model, ShapeId const id )
{
	assert( IsBaseKnotType(	model.GetShape( id )->GetShapeType() ) );
	m_idKnotStart = id;
	initialize( model );
}

void Pipeline::SetEndKnot( NNetModel & model, ShapeId const id )
{
	assert( IsBaseKnotType(	model.GetShape( id )->GetShapeType() ) );
	m_idKnotEnd = id;
	initialize( model );
}

MicroMeterPoint Pipeline::GetStartPoint( NNetModel const & model ) const 
{ 
	BaseKnot const * const pKnotStart { model.GetConstBaseKnot( m_idKnotStart ) };
	return pKnotStart ? pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipeline::GetEndPoint( NNetModel const & model ) const 
{ 
	BaseKnot const * const pKnotEnd { model.GetConstBaseKnot( m_idKnotEnd ) };   
	return pKnotEnd ? pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

MicroMeter Pipeline::GetWidth( ) const 
{ 
	return m_width; 
}

void Pipeline::Prepare( NNetModel & model )
{
	BaseKnot * const m_pKnotStart = model.GetBaseKnot( m_idKnotStart );   
	m_mVinputBuffer = m_pKnotStart->GetNextOutput( );
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );
}

void Pipeline::Step( )
{
	mV mVcarry = m_mVinputBuffer;
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );

	for ( auto & iter : m_potential )
	{
		std::swap( iter, mVcarry );
		assert( iter <= PEAK_VOLTAGE );
	}
}

mV Pipeline::GetNextOutput( ) const
{
	assert( m_potential.size() > 0 );
	assert( m_potential.back() <= PEAK_VOLTAGE );
	return m_potential.back();
}

bool Pipeline::IsPointInShape( NNetModel const & model, MicroMeterPoint const & point ) const
{
	MicroMeterPoint const fDelta { GetEndPoint( model ) - GetStartPoint( model ) };
	if ( IsCloseToZero( fDelta ) )
		return false;
	else
	{
		MicroMeterPoint const fOrthoScaled { OrthoVector( fDelta, m_width ) };
		MicroMeterPoint const corner1      { GetStartPoint( model ) + fOrthoScaled };
		MicroMeterPoint const corner2      { GetStartPoint( model ) - fOrthoScaled };
		MicroMeterPoint const corner3      { GetEndPoint  ( model ) + fOrthoScaled };
		return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
	}
}

void Pipeline::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	MicroMeterPoint const umStartPoint { model.GetConstBaseKnot( m_idKnotStart )->GetPosition() };
	MicroMeterPoint const umEndPoint   { model.GetConstBaseKnot( m_idKnotEnd   )->GetPosition() };
	if ( umStartPoint != umEndPoint )
	{
		fPIXEL      const fPixWidth  { coord.convert2fPixel( m_width ) };
		fPixelPoint const fStartPoint{ coord.convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { coord.convert2fPixelPos( umEndPoint   ) };
		COLORREF    const color      { IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ) };

		Graphics.StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
		Graphics.AddPipelinePoint( fEndPoint, color );
		Graphics.RenderPipeline( );
	}
}

void Pipeline::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	BaseKnot const * const pStartKnot   { model.GetConstBaseKnot( m_idKnotStart ) };
	BaseKnot const * const pEndKnot     { model.GetConstBaseKnot( m_idKnotEnd   ) };
	MicroMeterPoint        umStartPoint { pStartKnot->GetPosition() };
	MicroMeterPoint        umEndPoint   { pEndKnot  ->GetPosition() };
	if ( umStartPoint != umEndPoint )
	{
		MicroMeterPoint umVector { umEndPoint - umStartPoint };

		if ( pStartKnot->GetShapeType( ) != tShapeType::knot ) // pipeline stops at neuron border
		{
			MicroMeter umHypot = Hypot( umVector );
			assert( ! IsCloseToZero( umHypot ) );
			MicroMeterPoint const diff = umVector * (pStartKnot->GetExtension( ) * 0.79f / umHypot);
			umStartPoint += diff;
			umVector     -= diff;
		}

		MicroMeterPoint const segmentVector = umVector / CastToFloat(m_potential.size());
		MicroMeterPoint       umPoint       = umStartPoint;

		Graphics.StartPipeline
		( 
			coord.convert2fPixelPos( umStartPoint ), 
			coord.convert2fPixelPos( umEndPoint ), 
			coord.convert2fPixel   ( m_width ) * 0.6f, 
			pulseColor( * m_potential.begin() ) 
		);

		for ( auto & iter : m_potential )
		{
			umPoint += segmentVector; 
			Graphics.AddPipelinePoint
			( 
				coord.convert2fPixelPos( umPoint ), 
				pulseColor( iter ) 
			);
		}

		Graphics.RenderPipeline( );
	}
}

COLORREF Pipeline::pulseColor( mV const current ) const 
{
	assert( current <= PEAK_VOLTAGE );
	mV    const mVperColLevel = PEAK_VOLTAGE / 255.0f;
	int   const iLevel        = CastToInt( current / mVperColLevel );
	return RGB( iLevel, 0, 0 );
}

Pipeline const * Cast2Pipeline( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::pipeline );
	return static_cast<Pipeline const *>(shape);
}

Pipeline * Cast2Pipeline( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::pipeline );
	return static_cast<Pipeline *>(shape);
}
