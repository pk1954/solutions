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

bool BaseKnot::operator==( Shape const & rhs ) const
{
	BaseKnot const & baseKnotRhs { static_cast<BaseKnot const &>(rhs) };
	return 
	( this->Shape::operator== (rhs) )                    &&
	GetPos ().IsCloseTo(baseKnotRhs.GetPos ()) &&
	GetExtension().IsCloseTo(baseKnotRhs.GetExtension());
}

void BaseKnot::Dump() const
{
	Shape::Dump();
	wcout << m_connections << endl;
}

void BaseKnot::SetPos( MicroMeterPoint const & newPos )
{
	m_circle.SetPos( newPos ); 
	m_connections.Recalc();
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	SetPos( GetPos() + delta );
}

void BaseKnot::Link(Shape const & shapeSrc,	function<Shape * (Shape const *)> const & dstFromSrc)
{
	BaseKnot    const & baseKnotSrc { static_cast<BaseKnot const &>(shapeSrc) };
	Connections const & srcConn { baseKnotSrc.m_connections };
	m_connections.ClearOutgoing();
	m_connections.ClearIncoming();
	srcConn.Apply2AllOutPipes([&](Pipe const &p){m_connections.AddOutgoing(static_cast<Pipe *>(dstFromSrc(&p)));});
	srcConn.Apply2AllInPipes ([&](Pipe const &p){m_connections.AddIncoming(static_cast<Pipe *>(dstFromSrc(&p)));});
	if ( baseKnotSrc.GetParentShape() )
		SetParentShape(dstFromSrc(baseKnotSrc.GetParentShape()));
}

void BaseKnot::RotateShape( MicroMeterPoint const & umPntPivot, Radian const radDelta )
{
	MicroMeterPoint const umPntVectorOld    { GetPos() - umPntPivot };
	Radian          const radOld            { Vector2Radian(umPntVectorOld) };
	Radian          const radNew            { radOld + radDelta };
	MicroMeterPoint const umPntVectorNew    { Radian2Vector(radNew) };
	MicroMeter      const umDistFromPivot   { Hypot(umPntVectorOld) };
	MicroMeterPoint const umPntVectorScaled { umPntVectorNew.ScaledTo( umDistFromPivot) };
	MicroMeterPoint const umPntPosNew       { umPntPivot + umPntVectorScaled };
	SetPos( umPntPosNew );
}

bool const BaseKnot::IsIncludedIn( MicroMeterRect const & umRect ) const 
{ 
	return umRect.Includes( GetPos() ); 
}

void BaseKnot::Expand( MicroMeterRect & umRect ) const
{
	umRect.Expand( GetPos() );
}

void BaseKnot::Check() const
{
	Shape::Check();
	m_connections.Apply2AllInPipes ([&](Pipe & p) { assert(p.GetEndKnotId  () == GetId()); });
	m_connections.Apply2AllOutPipes([&](Pipe & p) { assert(p.GetStartKnotId() == GetId()); });
}

void BaseKnot::Prepare()
{
	m_mVinputBuffer = 0._mV;

	//for ( Pipe * pPipe : m_incoming ) 
	//{ 
	//	if ( pPipe != nullptr )
	//		m_mVinputBuffer += pPipe->GetNextOutput();
	//}

	m_connections.Apply2AllInPipes( [&]( Pipe & pipe ) { m_mVinputBuffer += pipe.GetNextOutput(); } ); // slow !!
}

bool const BaseKnot::IsPrecursorOf( Pipe const & pipeSucc ) const 
{
	return m_connections.Apply2AllOutPipesB( [&]( Pipe const & pipe ) { return & pipe == & pipeSucc; } ); 
}

bool const BaseKnot::IsSuccessorOf( Pipe const & pipePred ) const
{
	return m_connections.Apply2AllInPipesB( [&]( Pipe const & pipe ) { return & pipe == & pipePred; } );
}

bool const BaseKnot::Includes( MicroMeterPoint const & point ) const
{
	return Distance(point, GetPos()) <= GetExtension();
}

MicroMeterRect const BaseKnot::GetRect4Text() const
{
	return MicroMeterRect
	{
		GetPosX() - GetExtension(),      // left
		GetPosY() - GetExtension() / 2,  // top
		GetPosX() + GetExtension(),      // right
		GetPosY() + GetExtension()       // bottom
	};
}

void BaseKnot::drawCircle
(
	DrawContext      const & context, 
	D2D1::ColorF     const   colF, 
	MicroMeterCircle const   umCircle
) const
{
	context.FillCircle( umCircle, colF );
}

void BaseKnot::drawCircle
(
	DrawContext  const & context, 
	D2D1::ColorF const   colF, 
	MicroMeter   const   umWidth
) const
{
	context.FillCircle( MicroMeterCircle( GetPos(), umWidth ),	colF );
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
