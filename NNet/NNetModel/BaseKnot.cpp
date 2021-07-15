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

bool BaseKnot::operator==( Nob const & rhs ) const
{
	BaseKnot const & baseKnotRhs { static_cast<BaseKnot const &>(rhs) };
	return (this->Nob::operator==(rhs))             &&
	       GetPos().IsCloseTo(baseKnotRhs.GetPos()) &&
	       GetExtension().IsCloseTo(baseKnotRhs.GetExtension());
}

void BaseKnot::Dump() const
{
	Nob::Dump();
	m_connections.Dump();
	wcout << endl;
}

BaseKnot & BaseKnot::operator*=(float const f)
{
	m_circle *= f;
	return * this;
}

BaseKnot & BaseKnot::operator+=(BaseKnot const &rhs)
{
	m_circle += rhs.GetPos();
	return * this;
}

BaseKnot & BaseKnot::operator-=(BaseKnot const &rhs)
{
	m_circle += rhs.GetPos();
	return * this;
}

void BaseKnot::SetPos( MicroMeterPnt const & newPos )
{
	m_circle.SetPos( newPos ); 
	m_connections.Recalc();
}

void BaseKnot::MoveNob( MicroMeterPnt const & delta )
{
	SetPos(GetPos() + delta);
}

void BaseKnot::AddConnections(BaseKnot const & src) 
{ 
	m_connections.Add(src.m_connections);
	Reconnect();
}

void BaseKnot::SetConnections(BaseKnot const & src) 
{ 
	m_connections = src.m_connections;
	Reconnect();
}

void BaseKnot::SetConnections(Connections const & src) 
{ 
	m_connections = src;
	Reconnect();
}

void BaseKnot::ClearConnections()
{
	m_connections.ClearIncoming();
	m_connections.ClearOutgoing();
}

void BaseKnot::Link(Nob const & nobSrc,	Nob2NobFunc const & dstFromSrc)
{
	BaseKnot const & baseKnotSrc { static_cast<BaseKnot const &>(nobSrc) };
	ClearConnections();
	baseKnotSrc.Apply2AllOutPipes([&](Pipe const &p){AddOutgoing(static_cast<Pipe *>(dstFromSrc(&p)));});
	baseKnotSrc.Apply2AllInPipes ([&](Pipe const &p){AddIncoming(static_cast<Pipe *>(dstFromSrc(&p)));});
	if ( baseKnotSrc.GetParentNob() )
		SetParentNob(dstFromSrc(baseKnotSrc.GetParentNob()));
}

void BaseKnot::RotateNob( MicroMeterPnt const & umPntPivot, Radian const radDelta )
{
	MicroMeterPnt const umPntVectorOld    { GetPos() - umPntPivot };
	Radian        const radOld            { Vector2Radian(umPntVectorOld) };
	Radian        const radNew            { radOld + radDelta };
	MicroMeterPnt const umPntVectorNew    { Radian2Vector(radNew) };
	MicroMeter    const umDistFromPivot   { Hypot(umPntVectorOld) };
	MicroMeterPnt const umPntVectorScaled { umPntVectorNew.ScaledTo( umDistFromPivot) };
	MicroMeterPnt const umPntPosNew       { umPntPivot + umPntVectorScaled };
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
	Nob::Check();
	Apply2AllInPipes ([&](Pipe & p) { assert(p.GetEndKnotId  () == GetId()); });
	Apply2AllOutPipes([&](Pipe & p) { assert(p.GetStartKnotId() == GetId()); });
}

void BaseKnot::Prepare()
{
	m_mVinputBuffer = 0._mV;

	//for ( Pipe * pPipe : m_incoming ) 
	//{ 
	//	if ( pPipe != nullptr )
	//		m_mVinputBuffer += pPipe->GetNextOutput();
	//}

	Apply2AllInPipes([&](Pipe & pipe) { m_mVinputBuffer += pipe.GetNextOutput(); }); // slow !!
}

bool const BaseKnot::IsPrecursorOf( Pipe const & pipeSucc ) const 
{
	return m_connections.Apply2AllOutPipesB([&](Pipe const & pipe) { return & pipe == & pipeSucc; }); 
}

bool const BaseKnot::IsSuccessorOf( Pipe const & pipePred ) const
{
	return Apply2AllInPipesB([&](Pipe const & pipe) { return & pipe == & pipePred; });
}

bool const BaseKnot::Includes( MicroMeterPnt const & point ) const
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

BaseKnot const * Cast2BaseKnot( Nob const * nob )
{
	assert( ! nob->IsPipe() );
	assert( ! nob->IsUndefined() );
	return static_cast<BaseKnot const *>(nob);
}

BaseKnot * Cast2BaseKnot( Nob * nob )
{
	assert( ! nob->IsPipe() );
	assert( ! nob->IsUndefined() );
	return static_cast<BaseKnot *>(nob);
}
