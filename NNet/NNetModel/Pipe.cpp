// Pipe.cpp 
//
// NNetModel

#include "stdafx.h"
#include <cmath>
#include "assert.h"
#include "Geometry.h"
#include "PixelCoordsFp.h"
#include "tHighlightType.h"
#include "Direct2D.h"
#include "NNetParameters.h"
#include "Pipe.h"

using std::fill;

Pipe::Pipe( MicroMeterPoint const umUnused )
  :	Shape( ShapeType::Value::pipe ),
	m_pKnotStart ( nullptr ),
	m_pKnotEnd   ( nullptr ),
	m_width      ( PIPE_WIDTH ),
	m_potential  ( ),
	m_potIter    ( )
{
}

void Pipe::Clear( )
{
	LockShape();
	Shape::Clear( );
	fill( m_potential.begin(), m_potential.end(), 0.0_mV );
	UnlockShape();
}

void Pipe::Recalc( )
{
	if ( m_pKnotStart && m_pKnotEnd )
	{
		LockShape();
		meterPerSec  const pulseSpeed    { meterPerSec( m_pParameters->GetParameterValue( tParameter::pulseSpeed ) ) };
		MicroMeter   const segmentLength { CoveredDistance( pulseSpeed, m_pParameters->GetTimeResolution( ) ) };
		MicroMeter   const pipeLength    { Distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() ) };
		unsigned int const iNrOfSegments { max( 1, CastToUnsignedInt(round(pipeLength / segmentLength)) ) };
		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
		m_potIter = m_potential.begin();
		UnlockShape();
	}
}

void Pipe::SetStartKnot( BaseKnot * const pBaseKnot )
{
	m_pKnotStart = pBaseKnot;
	Recalc( );
}

void Pipe::SetEndKnot( BaseKnot * const pBaseKnot )
{
	m_pKnotEnd = pBaseKnot;
	Recalc( );
}

void Pipe::dislocate( BaseKnot * const pBaseKnot, MicroMeter const dislocation )
{ 
	MicroMeterPoint const umVector  { GetVector( ) };
	MicroMeterPoint const umNewPnt  { OrthoVector( umVector, dislocation ) };
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
	LockShape();
	MicroMeterPoint const umVector { GetEndPoint( ) - GetStartPoint( ) };
	if ( ! IsCloseToZero( umVector ) )
	{
		MicroMeterPoint const umSegVec     { umVector / CastToFloat(m_potential.size()) };
		MicroMeterPoint const umOrthoScaled{ OrthoVector( umVector, m_width ) };
		MicroMeterPoint       umPoint1     { GetStartPoint( ) };
		for ( auto iter = m_potIter; iter != m_potential.end( ); ++ iter )
		{
			MicroMeterPoint const umPoint2 { umPoint1 + umSegVec };
			if ( IsPointInRect2< MicroMeterPoint >( point, umPoint1, umPoint2, umOrthoScaled ) )
			{
				mVresult = * iter;
				goto EXIT;
			}
			umPoint1 = umPoint2;
		};
		for ( auto iter = m_potential.begin(); iter != m_potIter; ++iter )
		{
			MicroMeterPoint const umPoint2 { umPoint1 + umSegVec };
			if ( IsPointInRect2< MicroMeterPoint >( point, umPoint1, umPoint2, umOrthoScaled ) )
			{
				mVresult = * iter;
				goto EXIT;
			}
			umPoint1 = umPoint2;
		};
	}
EXIT:
	UnlockShape();
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

void Pipe::DrawExterior( D2D_driver const * pGraphics, PixelCoordsFp & coord, tHighlightType const type ) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	if ( umStartPoint != umEndPoint )
	{
		fPIXEL       const fPixWidth  { coord.convert2fPixel( m_width ) };
		fPixelPoint  const fStartPoint{ coord.convert2fPixelPos( umStartPoint ) };
		fPixelPoint  const fEndPoint  { coord.convert2fPixelPos( umEndPoint   ) };
		D2D1::ColorF const colF       { GetFrameColor( type ) };

		pGraphics->DrawLine( fStartPoint, fEndPoint, fPixWidth, colF );

		if ( m_arrowSize > 0.0_MicroMeter )
			pGraphics->DrawArrow
			(
				coord.convert2fPixelPos( (umEndPoint * 2.f + umStartPoint) / 3.f ), 
				coord.convert2fPixelSize( umEndPoint - umStartPoint ), 
				colF, 
				coord.convert2fPixel( m_arrowSize ),
				fPixWidth / 2 
			);
	}
}

void Pipe::drawSegment( D2D_driver const * pGraphics, fPixelPoint & fP1, fPixelPoint const fPixSegVec, fPIXEL const fWidth, mV const voltage ) const
{
	fPixelPoint  const fP2  { fP1 + fPixSegVec };
	D2D1::ColorF const colF { GetInteriorColor( voltage ) };
	pGraphics->DrawLine( fP1, fP2, fWidth, colF );
	fP1 = fP2;
}

void Pipe::DrawInterior( D2D_driver const * pGraphics, PixelCoordsFp & coord )
{
	MicroMeterPoint const umStartPoint { GetStartPoint( ) };
	MicroMeterPoint const umEndPoint   { GetEndPoint  ( ) };
	MicroMeterPoint const umVector     { umEndPoint - umStartPoint };
	if ( ! IsCloseToZero( umVector ) )
	{
		fPIXEL      const fWidth     { coord.convert2fPixel( m_width * PIPE_INTERIOR ) };
		fPixelPoint const fPixVector { coord.convert2fPixelSize( umVector ) };
		fPixelPoint const fPixSegVec { fPixVector / CastToFloat(m_potential.size()) };
		fPixelPoint       fPoint     { coord.convert2fPixelPos( umStartPoint ) };

		LockShape();
		for( auto iter = m_potIter + 1; iter != m_potential.end(); ++iter )
			drawSegment( pGraphics, fPoint, fPixSegVec, fWidth, * iter );             // fPoint altered!   
		for( auto iter = m_potential.begin(); iter <= m_potIter; ++iter )
			drawSegment( pGraphics, fPoint, fPixSegVec, fWidth, * iter );             // fPoint altered!
		//for( int i = m_potIter - m_potential.begin() + 1; i != m_potential.size(); ++i )
		//	drawSegment( pGraphics, fPoint, fPixSegVec, fWidth, m_potential[i] );             // fPoint altered!   
		//for( int i = 0; i <= m_potIter - m_potential.begin(); ++i )
		//	drawSegment( pGraphics, fPoint, fPixSegVec, fWidth, m_potential[i] );             // fPoint altered!
		UnlockShape();
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
