// Pipe.cpp 
//
// NNetModel

#include "stdafx.h"
#include <cmath>
#include <assert.h>
#include "Geometry.h"
#include "DrawContext.h"
#include "tHighlightType.h"
#include "NNetParameters.h"
#include "BaseKnot.h"
#include "Pipe.h"

using std::fixed;
using std::wstring;
using std::wostringstream;

#include <sstream> 
#include <iomanip>
using std::fill;
using std::wcout;
using std::endl;

Pipe::Pipe( BaseKnot * const pKnotStart, BaseKnot * const pKnotEnd )
  :	Shape( ShapeType::Value::pipe ),
	m_pKnotStart( pKnotStart ),
	m_pKnotEnd  ( pKnotEnd )
{
	assert( pKnotStart && pKnotEnd );
	Recalc();
}

Pipe::Pipe( Pipe const & src ) :  // copy constructor
	Shape       ( src ),
    m_pKnotStart( nullptr ),
	m_pKnotEnd  ( nullptr ),
	m_potIndex  ( src.m_potIndex  ),
	m_potential ( src.m_potential )
{ }

Pipe::~Pipe() { }

void Pipe::Dump() const
{
	Shape::Dump();
	wcout << L' ' << * this << endl;
}

void Pipe::init( const Pipe & rhs )
{
	Shape::operator=( rhs );
	m_pKnotStart = nullptr;
	m_pKnotEnd   = nullptr;
	m_potIndex   = rhs.m_potIndex;
	m_potential  = rhs.m_potential;
}

bool Pipe::operator==( Shape const & rhs ) const 
{
	Pipe const & pipeRhs { static_cast<Pipe const &>(rhs) };
	return
	( this->Shape::operator== (rhs) )                          && 
	( m_pKnotStart->GetId() == pipeRhs.m_pKnotStart->GetId() ) &&
	( m_pKnotEnd  ->GetId() == pipeRhs.m_pKnotEnd  ->GetId() );
}

ShapeId Pipe::GetStartKnotId() const 
{ 
	return m_pKnotStart->GetId(); 
}

ShapeId Pipe::GetEndKnotId() const 
{ 
	return m_pKnotEnd->GetId(); 
}

MicroMeterPoint const Pipe::GetPosition() const 
{ 
	return (m_pKnotStart->GetPosition() + m_pKnotEnd->GetPosition()) / 2.0f; 
}

void Pipe::Clear()
{
	Shape::Clear();
	fill( m_potential.begin(), m_potential.end(), 0.0_mV );
}

void Pipe::Recalc()
{
	if ( m_pKnotStart && m_pKnotEnd )
	{
		meterPerSec  const pulseSpeed    { meterPerSec( m_pParameters->GetParameterValue( ParamType::Value::pulseSpeed ) ) };
		MicroMeter   const segmentLength { CoveredDistance( pulseSpeed, m_pParameters->GetTimeResolution() ) };
		MicroMeter   const pipeLength    { Distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() ) };
		unsigned int const iNrOfSegments { max( 1, Cast2UnsignedInt(round(pipeLength / segmentLength)) ) };
		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
		m_potIndex = 0;
	}
}

void Pipe::CheckShape() const
{
	Shape::CheckShape();
	assert( m_pKnotStart->IsPrecursorOf( * this ) );
	assert( m_pKnotEnd  ->IsSuccessorOf( * this ) );
}

void Pipe::Expand( MicroMeterRect & umRect ) const
{
	umRect.Expand( GetStartPoint() );
	umRect.Expand( GetEndPoint  () );
}

void Pipe::Prepare()
{
	m_mVinputBuffer = m_pKnotStart->GetNextOutput();
}

void Pipe::MoveShape( MicroMeterPoint const & delta )
{
	m_pKnotStart->MoveShape( delta );
	m_pKnotEnd  ->MoveShape( delta );
}

// IsIncludedIn should be called IsPossiblyIncludedIn
// It doesn't calculate exactly if the pipe intersects umRect, but eliminites a lot of cases with a 
// simple and fast check. The rest is left over for the clipping algorithm of the graphics subsystem

bool const Pipe::IsIncludedIn( MicroMeterRect const & umRect ) const 
{ 
	if ( (m_pKnotStart->GetPosX() < umRect.GetLeft()) && (m_pKnotEnd->GetPosX() < umRect.GetLeft()) )
		return false;

	if ( (m_pKnotStart->GetPosX() > umRect.GetRight()) && (m_pKnotEnd->GetPosX() > umRect.GetRight()) )
		return false;

	if ( (m_pKnotStart->GetPosY() > umRect.GetBottom()) && (m_pKnotEnd->GetPosY() > umRect.GetBottom()) )
		return false;

	if ( (m_pKnotStart->GetPosY() < umRect.GetTop()) && (m_pKnotEnd->GetPosY() < umRect.GetTop()) )
		return false;

	return true;
}

void Pipe::SetStartKnot( BaseKnot * const pBaseKnot )
{
	assert( pBaseKnot );
	m_pKnotStart = pBaseKnot;
	Recalc();
}

void Pipe::SetEndKnot( BaseKnot * const pBaseKnot )
{
	assert( pBaseKnot );
	m_pKnotEnd = pBaseKnot;
	Recalc();
}

void Pipe::dislocate( BaseKnot * const pBaseKnot, MicroMeter const dislocation )
{ 
	pBaseKnot->MoveShape( GetVector().OrthoVector().ScaledTo( dislocation ) );
	Recalc();
}

MicroMeterPoint Pipe::GetStartPoint() const 
{ 
	return m_pKnotStart ? m_pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipe::GetEndPoint() const 
{ 
	return m_pKnotEnd ? m_pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

void Pipe::Select(bool const bOn, bool const bRecursive) 
{ 
	Shape::Select(bOn);
	if ( bRecursive )
	{
		if ( m_pKnotStart->IsKnot() )
			m_pKnotStart->Select(bOn, false);
		if ( m_pKnotEnd  ->IsKnot() )
			m_pKnotEnd->Select(bOn, false);
	}
}

MicroMeter Pipe::GetLength() const
{
	return Distance(GetStartPoint(), GetEndPoint());
}

bool const Pipe::Includes( MicroMeterPoint const & point ) const
{
	MicroMeterPoint const umVector{ GetEndPoint() - GetStartPoint() };
	if ( umVector.IsCloseToZero() )
		return false;
	MicroMeterPoint const umOrthoScaled{ umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
	MicroMeterPoint       umPoint1     { GetStartPoint() };
	MicroMeterPoint const umPoint2     { umPoint1 + umVector };
	return IsPointInRect2<MicroMeterPoint>( point, umPoint1, umPoint2, umOrthoScaled );
}

MicroMeterPoint Pipe::GetVector() const
{
	MicroMeterPoint const umStartPoint { GetStartPoint() };
	MicroMeterPoint const umEndPoint   { GetEndPoint  () };
	MicroMeterPoint const umvector{ umEndPoint - umStartPoint };
	assert( ! umvector.IsCloseToZero() );
	return umvector;
}

void Pipe::DrawArrows
( 
	DrawContext    const & context, 
	MicroMeter     const   umSize
) const
{
	MicroMeterPoint const umStartPoint { GetStartPoint() };
	MicroMeterPoint const umEndPoint   { GetEndPoint  () };

	context.FillArrow
	(
		(umEndPoint * 2.f + umStartPoint) / 3.f , 
		umEndPoint - umStartPoint, 
		umSize,
		PIPE_WIDTH / 2, 
		NNetColors::EXT_NORMAL
	);
}

void Pipe::DrawExterior( DrawContext const & context, tHighlight const type ) const
{
	context.DrawLine( GetStartPoint(), GetEndPoint(), PIPE_WIDTH, GetExteriorColor(type) );
}

void Pipe::DrawInterior( DrawContext const & context, tHighlight const type ) const
{
	static MicroMeter const umWidth { PIPE_WIDTH * PIPE_INTERIOR };

	if ( IsNormal(type) && ! IsSelected() )
	{
		MicroMeterPoint const umVector { GetEndPoint() - GetStartPoint() };
		if ( ! umVector.IsCloseToZero() )
		{
			size_t          const nrOfSegments { m_potential.size() };
			MicroMeterPoint const umSegVec     { umVector / Cast2Float(nrOfSegments) };
			MicroMeterPoint       umPoint      { GetStartPoint() };
			size_t          const potIndex     { m_potIndex };
			size_t                index        { potIndex }; 
			do 
			{
				if (++index == m_potential.size()) 
					index = 0; 

				MicroMeterPoint const umPointNext { umPoint + umSegVec };
				context.DrawLine( umPoint, umPointNext, umWidth, GetInteriorColor(m_potential[index]) );
				umPoint = umPointNext;
			} while (index != potIndex );
		}
	}
	else
	{
		context.DrawLine( GetStartPoint(), GetEndPoint(), umWidth, GetInteriorColor(type) );
	}
}

bool const Pipe::CompStep()
{
	m_potential[m_potIndex] = m_mVinputBuffer;
	if ( m_potIndex == 0 )
		m_potIndex = m_potential.size() - 1;  // caution!
	else                                      // modification if m_potIndex
		--m_potIndex;                         // must be atomic
	return false;
}

mV const Pipe::GetVoltage( MicroMeterPoint const & point ) const
{
	mV mVresult { 0._mV };
	MicroMeterPoint const umVector { GetEndPoint() - GetStartPoint() };
	if ( ! umVector.IsCloseToZero() )
	{
		size_t          const nrOfSegments  { m_potential.size() };
		MicroMeterPoint const umSegVec      { umVector / Cast2Float(nrOfSegments) };
		MicroMeterPoint const umOrthoScaled { umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
		MicroMeterPoint       umPoint       { GetStartPoint() };
		size_t          const potIndex      { m_potIndex };
		size_t                index         { potIndex }; 
		do 
		{
			if (++index == m_potential.size()) 
				index = 0; 

			MicroMeterPoint const umPoint2 { umPoint + umSegVec };
			if ( IsPointInRect2<MicroMeterPoint>( point, umPoint, umPoint2, umOrthoScaled ) )
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

wostream & operator<< ( wostream & out, Pipe const & pipe )
{
	out << Pipe::OPEN_BRACKET 
		<< pipe.GetStartKnotId() 
		<< Pipe::SEPARATOR
		<< pipe.GetEndKnotId() 
		<< Pipe::CLOSE_BRACKET;
	return out;
}
