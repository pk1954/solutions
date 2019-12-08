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

Pipeline::Pipeline( NNetModel * pModel )
  :	Shape( pModel, tShapeType::pipeline ),
	m_idKnotStart       ( NO_SHAPE ),
	m_idKnotEnd         ( NO_SHAPE ),
	m_width             ( PIPELINE_WIDTH ),
	m_fDampingPerSegment( 1.0f ),
	m_potential         ( )
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

		m_fDampingPerSegment = pow( m_pNNetModel->GetParameterValue( tParameter::dampingFactor ), segmentLength.GetValue() );
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
	m_mVinputBuffer = m_pKnotStart->GetNextOutput( );
	CheckInputBuffer( );
}

void Pipeline::Step( )
{
	CheckInputBuffer( );
	mV mVcarry { m_mVinputBuffer };
	for ( auto & iter : m_potential )
	{
		mVcarry *= m_fDampingPerSegment;  
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
	MicroMeterPoint const umStart { GetStartPoint( ) };
	MicroMeterPoint const umEnd   { GetEndPoint  ( ) };
	MicroMeterPoint const umvector{ umEnd - umStart };
	assert( ! IsCloseToZero( umvector ) );
	return umvector;
}

void Pipeline::DrawExterior( PixelCoordsFp & coord ) const
{
	MicroMeterPoint const umStartPoint{ m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotStart )->GetPosition() };
	MicroMeterPoint const umEndPoint  { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotEnd   )->GetPosition() };
	if ( umStartPoint != umEndPoint )
	{
		fPIXEL      const fPixWidth  { coord.convert2fPixel( m_width ) };
		fPixelPoint const fStartPoint{ coord.convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { coord.convert2fPixelPos( umEndPoint   ) };
		COLORREF    const color      { m_pNNetModel->GetFrameColor( * this ) };

		m_pGraphics->StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
		m_pGraphics->AddPipelinePoint( fEndPoint, color );
		m_pGraphics->RenderPipeline( );
	}
}

void Pipeline::DrawInterior( PixelCoordsFp & coord ) const
{
	BaseKnot const * const pStartKnot  { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotStart ) };
	BaseKnot const * const pEndKnot    { m_pNNetModel->GetConstTypedShape<BaseKnot>( m_idKnotEnd   ) };
	MicroMeterPoint        umStartPoint{ pStartKnot->GetPosition() };
	MicroMeterPoint        umEndPoint  { pEndKnot  ->GetPosition() };
	MicroMeterPoint        umVector    { umEndPoint - umStartPoint };
	if ( ! IsCloseToZero( umVector ) )
	{
		MicroMeterPoint const segmentVector = umVector / CastToFloat(m_potential.size());
		MicroMeterPoint       umPoint       = umStartPoint;

		m_pGraphics->StartPipeline
		( 
			coord.convert2fPixelPos( umStartPoint ), 
			coord.convert2fPixelPos( umEndPoint ), 
			coord.convert2fPixel   ( m_width * PIPELINE_INTERIOR ), 
			GetInteriorColor( * m_potential.begin() ) 
		);

		for ( auto & iter : m_potential )
		{
			umPoint += segmentVector; 
			m_pGraphics->AddPipelinePoint
			( 
				coord.convert2fPixelPos( umPoint ), 
				GetInteriorColor( iter ) 
			);
		}

		m_pGraphics->RenderPipeline( );
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
