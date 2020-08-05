// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "Geometry.h"
#include "DrawContext.h"
#include "Pipe.h"
#include "BaseKnot.h"

using std::find;
using std::begin;
using std::end;

void BaseKnot::SetPosition( MicroMeterPoint const & newPos )
{
	m_center = newPos;
	m_connections.Recalc();
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	SetPosition( m_center + delta );
}

void BaseKnot::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	
	//for ( Pipe * pPipe : m_incoming ) 
	//{ 
	//	if ( pPipe != nullptr )
	//		m_mVinputBuffer += pPipe->GetNextOutput( );
	//}

	m_connections.Apply2AllInPipes( [&]( auto pipe ) { m_mVinputBuffer += pipe.GetNextOutput( ); } ); // slow !!
}

bool BaseKnot::IsPrecursorOf( ShapeId const id )
{
	return m_connections.Apply2AllOutPipesB( [&]( Pipe & pipe ) { return pipe.GetEndKnotId( ) == id; } ); 
}

bool BaseKnot::IsSuccessorOf( ShapeId const id )
{
	return m_connections.Apply2AllInPipesB( [&]( Pipe & pipe ) { return pipe.GetStartKnotId( ) == id; } );
}

bool BaseKnot::IsPointInShape( MicroMeterPoint const & point ) const
{
	return Distance( point, m_center ) <= m_extension;
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
	DrawContext     const & context, 
	D2D1::ColorF    const   colF, 
	MicroMeterPoint const   umCenter,
	MicroMeter      const   umWidth
) const
{
	context.DrawCircle( umCenter, umWidth, colF );
}

void BaseKnot::drawCircle
(
	DrawContext  const & context, 
	D2D1::ColorF const   colF, 
	MicroMeter   const   umWidth
) const
{
	context.DrawCircle( GetPosition(), umWidth,	colF );
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
