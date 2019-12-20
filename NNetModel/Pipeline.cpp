// Pipeline.cpp 
//
// NNetModel

#include "stdafx.h"
#include <cmath>
#include "assert.h"
#include "Geometry.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Pipeline.h"

Pipeline::Pipeline( NNetModel * pModel )
  :	Shape( pModel, tShapeType::pipeline ),
	m_idKnotStart   ( NO_SHAPE ),
	m_idKnotEnd     ( NO_SHAPE ),
	m_width         ( PIPELINE_WIDTH ),
	m_fDampingFactor( 1.0f ),
	m_potential     ( )
{
}

void Pipeline::Recalc( )
{
	if ( ::IsDefined(m_idKnotStart) && ::IsDefined(m_idKnotEnd) )
	{
		BaseKnot const * const pKnotStart     { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotStart ) };   
		BaseKnot const * const pKnotEnd       { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotEnd   ) };   
		meterPerSec      const pulseSpeed     { meterPerSec( m_pNNetModel->GetParameterValue( tParameter::pulseSpeed ) ) };
		MicroMeter       const segmentLength  { CoveredDistance( pulseSpeed, TIME_RESOLUTION ) };
		MicroMeter       const pipelineLength { Distance( pKnotStart->GetPosition(), pKnotEnd->GetPosition() ) };
		unsigned int     const iNrOfSegments  { max( 1, CastToUnsignedInt(round(pipelineLength / segmentLength)) ) };

		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );

		m_fDampingFactor = pow( 1.0f - m_pNNetModel->GetParameterValue( tParameter::signalLoss ), segmentLength.GetValue() );
	}
}

void Pipeline::SetStartKnot( ShapeId const id )
{
	assert( m_pNNetModel->IsType<BaseKnot>( id ) );
	m_idKnotStart = id;
	Recalc( );
}

void Pipeline::SetEndKnot( ShapeId const id )
{
	assert( m_pNNetModel->IsType<BaseKnot>( id ) );
	m_idKnotEnd = id;
	Recalc( );
}

MicroMeterPoint Pipeline::GetStartPoint( ) const 
{ 
	BaseKnot const * const pKnotStart { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotStart ) };
	return pKnotStart ? pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipeline::GetEndPoint( ) const 
{ 
	BaseKnot const * const pKnotEnd { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotEnd ) };   
	return pKnotEnd ? pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

MicroMeter Pipeline::GetLength( ) const
{
	return Distance( GetStartPoint( ), GetEndPoint( ) );
}

void Pipeline::Prepare( )
{
	BaseKnot const * const m_pKnotStart { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotStart ) };
	if ( m_pKnotStart )
	{
		m_mVinputBuffer = m_pKnotStart->GetNextOutput( );
		CheckInputBuffer( );
	}
}

void Pipeline::Step( )
{
	CheckInputBuffer( );
	mV mVcarry { m_mVinputBuffer };
	for ( auto & iter : m_potential )
	{
		mVcarry *= m_fDampingFactor;  
		std::swap( iter, mVcarry );
		CheckInputBuffer( );
	}
}

mV Pipeline::GetNextOutput( ) const
{
	assert( m_potential.size() > 0 );
	assert( m_potential.back() <= mV( m_pNNetModel->GetParameterValue( tParameter::peakVoltage ) * 2 ) );
	return m_potential.back();
}

bool Pipeline::IsPointInShape( MicroMeterPoint const & point ) const
{
	MicroMeterPoint const fDelta{ GetEndPoint( ) - GetStartPoint( ) };
	if ( IsCloseToZero( fDelta ) )
		return false;
	else
	{
		MicroMeterPoint const fOrthoScaled{ OrthoVector( fDelta, m_width ) };
		MicroMeterPoint const corner1     { GetStartPoint( ) + fOrthoScaled };
		MicroMeterPoint const corner2     { GetStartPoint( ) - fOrthoScaled };
		MicroMeterPoint const corner3     { GetEndPoint  ( ) + fOrthoScaled };
		return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
	}
}

MicroMeterPoint Pipeline::GetVector( ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	MicroMeterPoint const umvector{ umEndPoint - umStartPoint };
	assert( ! IsCloseToZero( umvector ) );
	return umvector;
}

void Pipeline::DrawExterior( PixelCoordsFp & coord ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	if ( umStartPoint != umEndPoint )
	{
		fPIXEL      const fPixWidth  { coord.convert2fPixel( m_width ) };
		fPixelPoint const fStartPoint{ coord.convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { coord.convert2fPixelPos( umEndPoint   ) };
		COLORREF    const color      { m_pNNetModel->GetFrameColor( * this ) };

		m_pGraphics->DrawLine( fStartPoint, fEndPoint, fPixWidth, color );

		m_pGraphics->DrawArrow
		(
			coord.convert2fPixelPos( (umEndPoint + umStartPoint) / 2.f ), 
			coord.convert2fPixelSize( umEndPoint - umStartPoint ), 
			color, 
			coord.convert2fPixel( 30.0_MicroMeter ),
			fPixWidth / 2 
		);
	}
}

void Pipeline::DrawInterior( PixelCoordsFp & coord ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	MicroMeterPoint const umVector     { umEndPoint - umStartPoint };
	if ( ! IsCloseToZero( umVector ) )
	{
		MicroMeterPoint const segmentVector{ umVector / CastToFloat(m_potential.size()) };
		fPIXEL          const fWidth       { coord.convert2fPixel( m_width * PIPELINE_INTERIOR ) };
		MicroMeterPoint       umPoint      { umStartPoint };
		fPixelPoint           fPoint1      { coord.convert2fPixelPos( umPoint ) };

		for ( auto & iter : m_potential )
		{
			umPoint += segmentVector; 
			fPixelPoint fPoint2 { coord.convert2fPixelPos( umPoint ) };
			m_pGraphics->DrawLine
			( 
				fPoint1, fPoint2, fWidth,
				GetInteriorColor( iter ) 
			);
			fPoint1 = fPoint2;
		}
	}
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
