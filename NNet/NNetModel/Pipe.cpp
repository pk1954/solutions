// Pipe.cpp 
//
// NNetModel

#include "stdafx.h"
#include <cmath>
#include "assert.h"
#include "Geometry.h"
#include "DrawContext.h"
#include "tHighlightType.h"
#include "NNetParameters.h"
#include "Pipe.h"

using std::fill;

Pipe::Pipe( MicroMeterPoint const umUnused )
  :	Shape( ShapeType::Value::pipe )
{
	++ m_counter;
}

Pipe::~Pipe( )
{
	-- m_counter;
}

void Pipe::Clear( )
{
	LockShapeExclusive();
	Shape::Clear( );
	fill( m_potential.begin(), m_potential.end(), 0.0_mV );
	UnlockShapeExclusive();
}

void Pipe::Recalc( )
{
	if ( m_pKnotStart && m_pKnotEnd )
	{
		meterPerSec  const pulseSpeed    { meterPerSec( m_pParameters->GetParameterValue( tParameter::pulseSpeed ) ) };
		MicroMeter   const segmentLength { CoveredDistance( pulseSpeed, m_pParameters->GetTimeResolution( ) ) };
		MicroMeter   const pipeLength    { Distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() ) };
		unsigned int const iNrOfSegments { max( 1, CastToUnsignedInt(round(pipeLength / segmentLength)) ) };
		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
		m_potIter = m_potential.begin();
	}
}

void Pipe::SetStartKnot( BaseKnot * const pBaseKnot )
{
	m_pKnotStart = pBaseKnot;
}

void Pipe::SetEndKnot( BaseKnot * const pBaseKnot )
{
	m_pKnotEnd = pBaseKnot;
}

void Pipe::dislocate( BaseKnot * const pBaseKnot, MicroMeter const dislocation )
{ 
	MicroMeterPoint const umVector { GetVector( ) };
	MicroMeterPoint const umNewPnt { OrthoVector( umVector, dislocation ) };
	pBaseKnot->MoveShape( umNewPnt );
}

MicroMeterPoint Pipe::GetStartPoint( ) const 
{ 
	return m_pKnotStart ? m_pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipe::GetEndPoint( ) const 
{ 
	return m_pKnotEnd ? m_pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

MicroMeter Pipe::GetLength( ) const
{
	return Distance( GetStartPoint( ), GetEndPoint( ) );
}

bool Pipe::IsPointInShape( MicroMeterPoint const & point ) const
{
	MicroMeterPoint const umVector{ GetEndPoint( ) - GetStartPoint( ) };
	if ( IsCloseToZero( umVector ) )
		return false;
	MicroMeterPoint const umOrthoScaled{ OrthoVector( umVector, m_width ) };
	MicroMeterPoint       umPoint1     { GetStartPoint( ) };
	MicroMeterPoint const umPoint2     { umPoint1 + umVector };
	return IsPointInRect2<MicroMeterPoint>( point, umPoint1, umPoint2, umOrthoScaled );
}

mV Pipe::GetVoltage( MicroMeterPoint const & point ) const
{
	mV mVresult { 0._mV };
	LockShapeShared();
	MicroMeterPoint const umVector { GetEndPoint( ) - GetStartPoint( ) };
	if ( ! IsCloseToZero( umVector ) )
	{
		MicroMeterPoint const umSegVec     { umVector / CastToFloat(m_potential.size()) };
		MicroMeterPoint const umOrthoScaled{ OrthoVector( umVector, m_width ) };
		MicroMeterPoint       umPoint1     { GetStartPoint( ) };
		for ( tPotentialVector::const_iterator iter = m_potIter; iter != m_potIter; ++iter )
		{
			if ( iter == m_potential.end() )
				iter = m_potential.begin();
			MicroMeterPoint const umPoint2 { umPoint1 + umSegVec };
			if ( IsPointInRect2< MicroMeterPoint >( point, umPoint1, umPoint2, umOrthoScaled ) )
			{
				mVresult = * iter;
				break;
			}
			umPoint1 = umPoint2;
		};
	}
	UnlockShapeShared();
	return mVresult;
}

MicroMeterPoint Pipe::GetVector( ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	MicroMeterPoint const umvector{ umEndPoint - umStartPoint };
	assert( ! IsCloseToZero( umvector ) );
	return umvector;
}

void Pipe::DrawExterior( DrawContext const & context, tHighlightType const type ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	if ( umStartPoint != umEndPoint )
	{
		D2D1::ColorF const colF { GetFrameColor( type ) };

		context.DrawLine( umStartPoint, umEndPoint, m_width, colF );

		if ( m_arrowSize > 0.0_MicroMeter )
			context.DrawArrow
			(
				(umEndPoint * 2.f + umStartPoint) / 3.f , 
				umEndPoint - umStartPoint, 
				m_arrowSize,
				m_width / 2, 
				colF
			);
	}
}

MicroMeterPoint Pipe::drawSegment( DrawContext const & context, MicroMeterPoint const & umP1, MicroMeterPoint const & umPixSegVec, MicroMeter const umWidth, mV const voltage ) const
{
	MicroMeterPoint  const umP2  { umP1 + umPixSegVec };
	D2D1::ColorF const colF { GetInteriorColor( voltage ) };
	context.DrawLine( umP1, umP2, umWidth, colF );
	return umP2;
}

void Pipe::DrawInterior( DrawContext const & context ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	MicroMeterPoint const umVector     { umEndPoint - umStartPoint };
	if ( ! IsCloseToZero( umVector ) )
	{
		MicroMeter      const umWidth     { m_width * PIPE_INTERIOR };
		MicroMeterPoint const umPixSegVec { umVector / static_cast<float>(m_potential.size()) };
		MicroMeterPoint       umPoint     { umStartPoint };
//		mV                  * pPotential  { m_pPotential };  // m_pPotential can be modified by other thread
		tPotentialVector::iterator potIter { m_potIter };

		auto iter = potIter; 
		do 
		{
			if (++iter == m_potential.end()) 
				iter = potIter; 
			umPoint = drawSegment( context, umPoint, umPixSegVec, umWidth, * iter );
		} while (iter != potIter );
	}
}

Pipe const * Cast2Pipe( Shape const * pShape )
{
	assert( pShape->IsPipe() );
	return static_cast<Pipe const *>(pShape);
}

Pipe * Cast2Pipe( Shape * pShape )
{
	assert( pShape->IsPipe() );
	return static_cast<Pipe *>(pShape);
}
