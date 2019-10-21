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
	m_width       ( PIPELINE_WIDTH ),
	m_potential   ( ),
	m_impulseSpeed( impulseSpeed )
{
}

void Pipeline::Resize( )
{
	if ( IsDefined(m_idKnotStart)  && IsDefined(m_idKnotEnd) )
	{
		MicroMeter const segmentLength  = CoveredDistance( m_impulseSpeed, TIME_RESOLUTION );
		BaseKnot * const m_pKnotStart   = m_pModel->GetBaseKnot( m_idKnotStart );   
		BaseKnot * const m_pKnotEnd     = m_pModel->GetBaseKnot( m_idKnotEnd   );   
		MicroMeter const pipelineLength = Distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() );
		unsigned int     iNrOfSegments  = CastToUnsignedInt(round(pipelineLength / segmentLength));

		if ( iNrOfSegments == 0 )
			iNrOfSegments = 1;

		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
		m_initialized = true;
	}
}

void Pipeline::initialize( )
{
	if ( ! m_initialized  )
		Resize( );
}

void Pipeline::SetStartKnot( ShapeId const id )
{
	assert( IsBaseKnotType(	m_pModel->GetShape( id )->GetShapeType() ) );
	m_idKnotStart = id;
	initialize( );
}

void Pipeline::SetEndKnot( ShapeId const id )
{
	assert( IsBaseKnotType(	m_pModel->GetShape( id )->GetShapeType() ) );
	m_idKnotEnd = id;
	initialize( );
}

MicroMeterPoint Pipeline::GetStartPoint( ) const 
{ 
	BaseKnot const * const pKnotStart { m_pModel->GetConstBaseKnot( m_idKnotStart ) };
	return pKnotStart ? pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipeline::GetEndPoint( ) const 
{ 
	BaseKnot const * const pKnotEnd { m_pModel->GetConstBaseKnot( m_idKnotEnd ) };   
	return pKnotEnd ? pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

MicroMeter Pipeline::GetWidth( ) const 
{ 
	return m_width; 
}

void Pipeline::Prepare( )
{
	BaseKnot * const m_pKnotStart = m_pModel->GetBaseKnot( m_idKnotStart );   
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

bool Pipeline::IsPointInShape( MicroMeterPoint const & point ) const
{
	MicroMeterPoint const fDelta { GetEndPoint( ) - GetStartPoint( ) };
	if ( IsCloseToZero( fDelta ) )
		return false;
	else
	{
		MicroMeterPoint const fOrthoScaled { OrthoVector( fDelta, m_width ) };
		MicroMeterPoint const corner1      { GetStartPoint( ) + fOrthoScaled };
		MicroMeterPoint const corner2      { GetStartPoint( ) - fOrthoScaled };
		MicroMeterPoint const corner3      { GetEndPoint  ( ) + fOrthoScaled };
		return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
	}
}

void Pipeline::DrawExterior( ) const
{
	MicroMeterPoint const umStartPoint { m_pModel->GetConstBaseKnot( m_idKnotStart )->GetPosition() };
	MicroMeterPoint const umEndPoint   { m_pModel->GetConstBaseKnot( m_idKnotEnd   )->GetPosition() };
	if ( umStartPoint != umEndPoint )
	{
		fPIXEL      const fPixWidth  { m_pCoord->convert2fPixel( m_width ) };
		fPixelPoint const fStartPoint{ m_pCoord->convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { m_pCoord->convert2fPixelPos( umEndPoint   ) };
		COLORREF    const color      { IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ) };

		m_pGraphics->StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
		m_pGraphics->AddPipelinePoint( fEndPoint, color );
		m_pGraphics->RenderPipeline( );
	}
}

void Pipeline::DrawInterior( ) const
{
	BaseKnot const * const pStartKnot   { m_pModel->GetConstBaseKnot( m_idKnotStart ) };
	BaseKnot const * const pEndKnot     { m_pModel->GetConstBaseKnot( m_idKnotEnd   ) };
	MicroMeterPoint        umStartPoint { pStartKnot->GetPosition() };
	MicroMeterPoint        umEndPoint   { pEndKnot  ->GetPosition() };
	MicroMeterPoint        umVector     { umEndPoint - umStartPoint };
	if ( ! IsCloseToZero( umVector ) )
	{
		if ( pStartKnot->GetShapeType( ) != tShapeType::knot ) // pipeline stops at neuron border
		{
			MicroMeter      const umHypot { Hypot( umVector ) };
			//MicroMeterPoint const diff    { umVector * (pStartKnot->GetExtension( ) * NEURON_INTERIOR / umHypot) };
			//umStartPoint += diff;
			//umVector     -= diff;
		}

		MicroMeterPoint const segmentVector = umVector / CastToFloat(m_potential.size());
		MicroMeterPoint       umPoint       = umStartPoint;

		m_pGraphics->StartPipeline
		( 
			m_pCoord->convert2fPixelPos( umStartPoint ), 
			m_pCoord->convert2fPixelPos( umEndPoint ), 
			m_pCoord->convert2fPixel   ( m_width * PIPELINE_INTERIOR ), 
			pulseColor( * m_potential.begin() ) 
		);

		for ( auto & iter : m_potential )
		{
			umPoint += segmentVector; 
			m_pGraphics->AddPipelinePoint
			( 
				m_pCoord->convert2fPixelPos( umPoint ), 
				pulseColor( iter ) 
			);
		}

		m_pGraphics->RenderPipeline( );
	}
}

COLORREF Pipeline::pulseColor( mV const current ) const 
{
	assert( current <= PEAK_VOLTAGE );
	mV    const mVperColLevel = PEAK_VOLTAGE / 255.0f;
	int   const iLevel        = CastToInt( current / mVperColLevel );
	return RGB( iLevel, 0, 0 );
}

void Pipeline::CheckConsistency( ) const
{
	Shape const * pShapeStartKnot = m_pModel->GetConstShape( m_idKnotStart );
	Shape const * pShapeEndKnot   = m_pModel->GetConstShape( m_idKnotEnd );
	tShapeType typeStartKnot = pShapeStartKnot->GetShapeType();
	tShapeType typeEndKnot   = pShapeEndKnot->GetShapeType();
	assert(
		(typeStartKnot == tShapeType::knot)        || 
		(typeStartKnot == tShapeType::inputNeuron) || 
		(typeStartKnot == tShapeType::neuron)
	);
	assert(
		(typeEndKnot == tShapeType::knot)         || 
		(typeEndKnot == tShapeType::outputNeuron) || 
		(typeEndKnot == tShapeType::neuron)
	);
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
