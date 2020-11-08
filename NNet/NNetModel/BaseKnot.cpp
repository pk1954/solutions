// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "Geometry.h"
#include "DrawContext.h"
#include "Pipe.h"
#include "BaseKnot.h"

using std::wcout;
using std::endl;
using std::find;
using std::begin;
using std::end;

BaseKnot::BaseKnot( BaseKnot const & src ) :  // copy constructor
	Shape        ( src ),
	m_circle     ( src.m_circle ),
	m_connections( )    // do not copy connections!
{ 
	MoveShape( PIPE_WIDTH ); 
}

bool BaseKnot::operator==( Shape const & rhs ) const
{
	BaseKnot const & baseKnotRhs { static_cast<BaseKnot const &>(rhs) };
	return 
	( this->Shape::operator== (rhs) )                                &&
	( IsCloseToZero( GetPosition () - baseKnotRhs.GetPosition () ) ) &&
	( IsCloseToZero( GetExtension() - baseKnotRhs.GetExtension() ) );
}

void BaseKnot::Dump( ) const
{
	Shape::Dump();
	wcout << m_connections << endl;
}


void BaseKnot::SetPosition( MicroMeterPoint const & newPos )
{
	m_circle.SetPosition( newPos ); 
	m_connections.Recalc();
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	SetPosition( GetPosition() + delta );
}

void BaseKnot::CheckShape( ) const
{
	Shape::CheckShape();
	m_connections.Apply2AllInPipes ( [&]( Pipe & pipe ) { assert( pipe.GetEndKnotId  () == GetId() ); } );
	m_connections.Apply2AllOutPipes( [&]( Pipe & pipe ) { assert( pipe.GetStartKnotId() == GetId() ); } );
}

void BaseKnot::Prepare( )
{
	m_mVinputBuffer = 0._mV;

	//for ( Pipe * pPipe : m_incoming ) 
	//{ 
	//	if ( pPipe != nullptr )
	//		m_mVinputBuffer += pPipe->GetNextOutput( );
	//}

	m_connections.Apply2AllInPipes( [&]( Pipe & pipe ) { m_mVinputBuffer += pipe.GetNextOutput( ); } ); // slow !!
}

bool BaseKnot::IsPrecursorOf( Pipe const & pipeSucc ) const 
{
	return m_connections.Apply2AllOutPipesB( [&]( Pipe const & pipe ) { return & pipe == & pipeSucc; } ); 
}

bool BaseKnot::IsSuccessorOf( Pipe const & pipePred ) const
{
	return m_connections.Apply2AllInPipesB( [&]( Pipe const & pipe ) { return & pipe == & pipePred; } );
}

bool BaseKnot::IsPointInShape( MicroMeterPoint const & point ) const
{
	return Distance( point, GetPosition() ) <= m_circle.GetRadius();
}

MicroMeterRect const BaseKnot::GetRect4Text( ) const
{
	return MicroMeterRect
	{
		GetPosition().GetX() - GetExtension(),      // left
		GetPosition().GetY() - GetExtension() / 2,  // top
		GetPosition().GetX() + GetExtension(),      // right
		GetPosition().GetY() + GetExtension()       // bottom
	};
}

void BaseKnot::drawCircle
(
	DrawContext      const & context, 
	D2D1::ColorF     const   colF, 
	MicroMeterCircle const   umCircle
) const
{
	context.DrawCircle( umCircle, colF );
}

void BaseKnot::drawCircle
(
	DrawContext  const & context, 
	D2D1::ColorF const   colF, 
	MicroMeter   const   umWidth
) const
{
	context.DrawCircle( MicroMeterCircle( GetPosition(), umWidth ),	colF );
}

BaseKnot const * Cast2BaseKnot( Shape const * shape )
{
	assert( ! shape->IsPipe() );
	assert( ! shape->IsUndefined() );
	return static_cast<BaseKnot const *>(shape);
}

BaseKnot * Cast2BaseKnot( Shape * shape )
{
	assert( ! shape->IsPipe() );
	assert( ! shape->IsUndefined() );
	return static_cast<BaseKnot *>(shape);
}
