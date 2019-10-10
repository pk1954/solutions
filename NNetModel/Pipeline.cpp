// Pipeline.cpp 
//
// NNetModel

#include "stdafx.h"
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
	BaseKnot const * const m_pKnotStart { model.GetConstBaseKnot( m_idKnotStart ) };
	return m_pKnotStart ? m_pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipeline::GetEndPoint( NNetModel const & model ) const 
{ 
	BaseKnot const * const m_pKnotEnd { model.GetConstBaseKnot( m_idKnotEnd ) };   
	return m_pKnotEnd ? m_pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

MicroMeter Pipeline::GetWidth( ) const 
{ 
	return m_width; 
}

void Pipeline::Prepare( NNetModel & model )
{
	BaseKnot * const m_pKnotStart = model.GetBaseKnot( m_idKnotStart );   
	m_mVinputBuffer = m_pKnotStart->GetNextOutput( );
}

void Pipeline::Step( )
{
	mV mVcarry = m_mVinputBuffer;

	for ( vector<mV>::iterator iter = m_potential.begin( ); iter != m_potential.end( ); iter++ )
	{
		std::swap( * iter, mVcarry );
	}
}

mV Pipeline::GetNextOutput( ) const
{
	assert( m_potential.size() > 0 );
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

void Pipeline::Draw
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	MicroMeter      const startOffset = ( model.GetConstBaseKnot( m_idKnotStart )->GetShapeType() == tShapeType::knot ) 
		                                ? 0.0_MicroMeter
		                                : model.GetConstBaseKnot( m_idKnotStart )->GetExtension( ) * 0.8f;
	MicroMeterPoint const startPoint  = GetStartPoint( model ); // + MicroMeterPoint( 0._MicroMeter, startOffset );
	fPIXEL          const fPixWidth   = coord.convert2fPixel( m_width ) ;

	///// draw border

	fPixelPoint const fStartPoint = coord.convert2fPixelPos( startPoint );
	fPixelPoint const fEndPoint   = coord.convert2fPixelPos( GetEndPoint( model ) );
	COLORREF    const colorBorder = IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 0, 255 );
	Graphics.AddfPixelLine( fStartPoint, fEndPoint, fPixWidth, colorBorder );

	///// draw interior

	MicroMeterPoint const vector        = GetEndPoint( model ) - startPoint;
	MicroMeterPoint const segmentVector = vector / CastToFloat(m_potential.size());
	fPixelPoint           fPixPoint1    = coord.convert2fPixelPos( startPoint );
	MicroMeterPoint       point2        = startPoint + segmentVector;

	for ( std::vector<mV>::const_iterator iter = m_potential.begin( ); iter != m_potential.end( ); iter++ )
	{
		assert( * iter <= PEAK_VOLTAGE );
		mV              const mVperColLevel = PEAK_VOLTAGE / 255.0f;
		int             const iLevel        = CastToInt( * iter / mVperColLevel );
		fPixelPoint     const fPixPoint2    = coord.convert2fPixelPos( point2 );
		COLORREF        const color         = RGB( iLevel, 0, 0 );
		Graphics.AddfPixelLine( fPixPoint1, fPixPoint2, fPixWidth * 0.6f, color );
		point2    += segmentVector; 
		fPixPoint1 = fPixPoint2;
	}

	///// draw end connector

	Graphics.DrawCircle( fEndPoint, RGB( 255,  50,  50 ), fPixWidth );
}
