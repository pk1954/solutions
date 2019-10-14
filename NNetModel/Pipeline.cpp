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

	MicroMeter Pipeline::distance( MicroMeterPoint const & npA, MicroMeterPoint const & npB )
{
	MicroMeterPoint npDiff   = npA - npB;
	MicroMeter      distance = MicroMeter
	(
		sqrt
		( 
			npDiff.GetXvalue() * npDiff.GetXvalue() + 
			npDiff.GetYvalue() * npDiff.GetYvalue() 
		)
	);
	return distance;
}

void Pipeline::Resize( NNetModel & model )
{
	if ( IsDefined(m_idKnotStart)  && IsDefined(m_idKnotEnd) )
	{
		MicroMeter const segmentLength  = CoveredDistance( m_impulseSpeed, TIME_RESOLUTION );
		BaseKnot * const m_pKnotStart   = model.GetBaseKnot( m_idKnotStart );   
		BaseKnot * const m_pKnotEnd     = model.GetBaseKnot( m_idKnotEnd   );   
		MicroMeter const pipelineLength = distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() );
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

void Pipeline::MoveTo( NNetModel & model, MicroMeterPoint const & newCenter )
{
//	m_center = newCenter;
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

	for ( vector<mV>::iterator iter = m_potential.begin( ); iter != m_potential.end( ); iter++ )
	{
		std::swap( * iter, mVcarry );
		assert( * iter <= PEAK_VOLTAGE );
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
	MicroMeterPoint fDelta { GetEndPoint( model )  - GetStartPoint( model ) };
	MicroMeterPoint fOrtho { fDelta.GetY(), - fDelta.GetX()   };

	float const fScaleFactor = m_width.GetValue() / sqrt( fOrtho.GetXvalue() * fOrtho.GetXvalue() + fOrtho.GetYvalue() * fOrtho.GetYvalue() );

	MicroMeterPoint fOrthoScaled = fOrtho * fScaleFactor;

	MicroMeterPoint const corner1 = GetStartPoint( model ) + fOrthoScaled;
	MicroMeterPoint const corner2 = GetStartPoint( model ) - fOrthoScaled;
	MicroMeterPoint const corner3 = GetEndPoint  ( model ) + fOrthoScaled;

	return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
}

void Pipeline::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	fPIXEL           const fPixWidth  { coord.convert2fPixel( m_width ) };
	BaseKnot const * const pStartKnot { model.GetConstBaseKnot( m_idKnotStart ) };
	BaseKnot const * const pEndKnot   { model.GetConstBaseKnot( m_idKnotEnd   ) };
	MicroMeterPoint        startPnt   { pStartKnot->GetPosition() };
	MicroMeterPoint        endPnt     { pEndKnot  ->GetPosition() };
	MicroMeterPoint        vector     { endPnt - startPnt };

	if ( pStartKnot->GetShapeType( ) != tShapeType::knot ) 
	{
		float           const extension = pStartKnot->GetExtension( ).GetValue() * 0.8f;
		float           const factor    = hypotf( vector.GetXvalue(), vector.GetYvalue() );
		MicroMeterPoint const diff      = vector * (extension / factor) * 0.99f;
		startPnt  += diff;
		vector    -= diff;
	}

	fPixelPoint const fStartPoint = coord.convert2fPixelPos( startPnt );
	fPixelPoint const fEndPoint   = coord.convert2fPixelPos( endPnt );
	COLORREF    const color       = IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 );

	Graphics.StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
	Graphics.AddPipelinePoint( fEndPoint, color );
	Graphics.RenderPipeline( );
}

void Pipeline::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	fPIXEL           const fPixWidth  { coord.convert2fPixel( m_width ) * 0.6f };
	BaseKnot const * const pStartKnot { model.GetConstBaseKnot( m_idKnotStart ) };
	BaseKnot const * const pEndKnot   { model.GetConstBaseKnot( m_idKnotEnd   ) };
	MicroMeterPoint        startPnt   { pStartKnot->GetPosition() };
	MicroMeterPoint        endPnt     { pEndKnot  ->GetPosition() };
	MicroMeterPoint        vector     { endPnt - startPnt };

	if ( pStartKnot->GetShapeType( ) != tShapeType::knot ) 
	{
		float           const extension = pStartKnot->GetExtension( ).GetValue() * 0.8f;
		float           const factor    = hypotf( vector.GetXvalue(), vector.GetYvalue() );
		MicroMeterPoint const diff      = vector * (extension / factor) * 0.99f;
		startPnt  += diff;
		vector    -= diff;
	}

	fPixelPoint     const fEndPoint     = coord.convert2fPixelPos( endPnt );
	MicroMeterPoint const segmentVector = vector / CastToFloat(m_potential.size());
	fPixelPoint           fPixPoint1    = coord.convert2fPixelPos( startPnt );
	MicroMeterPoint       umPoint       = startPnt;

	Graphics.StartPipeline( fPixPoint1, fEndPoint, fPixWidth, pulseColor( * m_potential.begin() ) );

	for ( std::vector<mV>::const_iterator iter = m_potential.begin( ); iter != m_potential.end( ); iter++ )
	{
		umPoint += segmentVector; 
		fPixelPoint const fPixPoint2 = coord.convert2fPixelPos( umPoint );
		Graphics.AddPipelinePoint( fPixPoint2, pulseColor( * iter ) );
	}

	Graphics.RenderPipeline( );
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
