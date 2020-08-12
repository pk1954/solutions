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
#include "BaseKnot.h"
#include "Pipe.h"

using std::fill;

Pipe::Pipe( BaseKnot * const pKnotStart, BaseKnot * const pKnotEnd )
  :	Shape( ShapeType::Value::pipe ),
	m_pKnotStart( pKnotStart ),
	m_pKnotEnd  ( pKnotEnd )
{
	assert( pKnotStart && pKnotEnd );
	Recalc();
}

Pipe::~Pipe( ) { }

bool Pipe::IsEqual( Pipe const & other ) const 
{
	if ( ! Shape::IsEqual( other ) )
		return false;
	if ( m_pKnotStart->GetId() != other.m_pKnotStart->GetId() )
		return false;
	if ( m_pKnotEnd->GetId() != other.m_pKnotEnd->GetId() )
		return false;
	if ( m_width != other.m_width )
		return false;
	return true;
}

ShapeId Pipe::GetStartKnotId() const 
{ 
	return m_pKnotStart->GetId(); 
}

ShapeId Pipe::GetEndKnotId() const 
{ 
	return m_pKnotEnd->GetId(); 
}

void Pipe::Clear( )
{
	Shape::Clear( );
	fill( m_potential.begin(), m_potential.end(), 0.0_mV );
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
		m_potIndex = 0;
	}
}

void Pipe::Prepare( )
{
	m_mVinputBuffer = m_pKnotStart->GetNextOutput( );
}

void Pipe::Select( tBoolOp const op ) 
{ 
	Shape::Select( op );
	m_pKnotStart->Select( op );
	m_pKnotEnd  ->Select( op );
}

void Pipe::MoveShape( MicroMeterPoint const & delta )
{
	m_pKnotStart->MoveShape( delta );
	m_pKnotEnd  ->MoveShape( delta );
}

// IsInrect should be called IsPossiblyInRect
// It doesn't calculate exectly if the pipe intersects umRect, but eliminites a lot of cases with a simple and fast check
// The rest is left over for the clipping algorithm of the graphics subsystem

bool Pipe::IsInRect( MicroMeterRect const & umRect ) const 
{ 
	if ( (m_pKnotStart->GetPosition().GetX() < umRect.GetLeft()) && (m_pKnotEnd->GetPosition().GetX() < umRect.GetLeft()) )
		return false;

	if ( (m_pKnotStart->GetPosition().GetX() > umRect.GetRight()) && (m_pKnotEnd->GetPosition().GetX() > umRect.GetRight()) )
		return false;

	if ( (m_pKnotStart->GetPosition().GetY() > umRect.GetBottom()) && (m_pKnotEnd->GetPosition().GetY() > umRect.GetBottom()) )
		return false;

	if ( (m_pKnotStart->GetPosition().GetY() < umRect.GetTop()) && (m_pKnotEnd->GetPosition().GetY() < umRect.GetTop()) )
		return false;

	return true;
}

void Pipe::SetStartKnot( BaseKnot * const pBaseKnot )
{
	m_pKnotStart = pBaseKnot;
	Recalc();
}

void Pipe::SetEndKnot( BaseKnot * const pBaseKnot )
{
	m_pKnotEnd = pBaseKnot;
	Recalc();
}

void Pipe::dislocate( BaseKnot * const pBaseKnot, MicroMeter const dislocation )
{ 
	MicroMeterPoint const umVector { GetVector( ) };
	MicroMeterPoint const umNewPnt { OrthoVector( umVector, dislocation ) };
	pBaseKnot->MoveShape( umNewPnt );
	Recalc( );
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

void Pipe::DrawInterior( DrawContext const & context ) const
{
	MicroMeterPoint const umVector { GetEndPoint( ) - GetStartPoint( ) };
	if ( ! IsCloseToZero( umVector ) )
	{
		size_t          const nrOfSegments { m_potential.size() };
		MicroMeterPoint const umSegVec     { umVector / CastToFloat(nrOfSegments) };
		MicroMeter      const umWidth      { m_width * PIPE_INTERIOR };
		MicroMeterPoint       umPoint      { GetStartPoint( ) };
		size_t          const potIndex     { m_potIndex };
		size_t                index        { potIndex }; 
		do 
		{
			if (++index == m_potential.size()) 
				index = 0; 

			MicroMeterPoint const umPointNext { umPoint + umSegVec };
			context.DrawLine( umPoint, umPointNext, umWidth, GetInteriorColor( m_potential[index] ) );
			umPoint = umPointNext;

		} while (index != potIndex );
	}
}

mV Pipe::GetVoltage( MicroMeterPoint const & point ) const
{
	mV mVresult { 0._mV };
	MicroMeterPoint const umVector { GetEndPoint( ) - GetStartPoint( ) };
	if ( ! IsCloseToZero( umVector ) )
	{
		size_t          const nrOfSegments  { m_potential.size() };
		MicroMeterPoint const umSegVec      { umVector / CastToFloat(nrOfSegments) };
		MicroMeterPoint const umOrthoScaled { OrthoVector( umVector, m_width ) };
		MicroMeterPoint       umPoint       { GetStartPoint( ) };
		size_t          const potIndex      { m_potIndex };
		size_t                index         { potIndex }; 
		do 
		{
			if (++index == m_potential.size()) 
				index = 0; 

			MicroMeterPoint const umPoint2 { umPoint  + umSegVec };
			if ( IsPointInRect2< MicroMeterPoint >( point, umPoint, umPoint2, umOrthoScaled ) )
			{
				mVresult = m_potential[index];
				break;
			}
			umPoint = umPoint2;

		} while (index != potIndex );
	}
	return mVresult;
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
