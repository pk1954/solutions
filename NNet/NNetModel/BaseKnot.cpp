// BaseKnot.cpp
//
// NNetModel

module;

#include <cassert>
#include <functional>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Resource.h"

module NNetModel:BaseKnot;

import Types;
import DrawContext;
import :NNetColors;
import :Pipe;
import :IoLine;
import :NobException;
import :NobType;
import :Nob;

using std::wcout;
using std::endl;
using std::find;
using std::begin;
using std::end;
using std::vector;
using std::function;

BaseKnot::BaseKnot
(
	MicroMeterPnt const & center,
	NobType       const   type,
	MicroMeter    const   extension
)
  : Nob(type),
	m_circle(center, extension)
{ }

bool BaseKnot::operator==(Nob const & rhs) const
{
	BaseKnot const & baseKnotRhs { static_cast<BaseKnot const &>(rhs) };
	return (this->Nob::operator==(rhs))                &&
	       GetPos().IsCloseTo(baseKnotRhs.GetPos()) &&
	       GetExtension().IsCloseTo(baseKnotRhs.GetExtension());
}

void BaseKnot::Dump() const
{
	Nob::Dump();
	wcout << L" in";
	m_inPipes.Dump();
	wcout << L" out";
	m_outPipes.Dump();
	wcout << endl;
}

void BaseKnot::EvaluateSelectionStatus()
{
	bool bSelected { Apply2AllConnectedPipesB([](Pipe const& p) { return p.IsSelected(); }) };
	Nob::Select(bSelected);      // if any connected pipe is selected
}                                // knot must also be selected

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

void BaseKnot::SetPos(MicroMeterPnt const & newPos)
{
	m_circle.SetPos(newPos);
	m_inPipes .Recalc();
	m_outPipes.Recalc();
}

void BaseKnot::MoveNob(MicroMeterPnt const & delta)
{
	SetPos(GetPos() + delta);
}

void BaseKnot::Reconnect() 
{ 
	m_inPipes .Apply2All([this](Pipe & pipe) { pipe.SetEndPnt  (this); });
	m_outPipes.Apply2All([this](Pipe & pipe) { pipe.SetStartPnt(this); });
}

void BaseKnot::Link(Nob const & nobSrc,	Nob2NobFunc const & f)
{
	BaseKnot const & src { static_cast<BaseKnot const &>(nobSrc) };
	m_inPipes.Clear();
	m_outPipes.Clear();
	src.Apply2AllOutPipes([this,f](Pipe const & p){AddOutgoing(static_cast<Pipe &>(*f(&p)));});
	src.Apply2AllInPipes ([this,f](Pipe const & p){AddIncoming(static_cast<Pipe &>(*f(&p)));});
	if (src.GetParentNob())
		SetParentNob(f(src.GetParentNob()));
}

void BaseKnot::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
	m_circle.Rotate(umPntPivot, radDelta);
}

bool BaseKnot::IsIncludedIn(MicroMeterRect const & umRect) const 
{ 
	return umRect.Includes(GetPos()); 
}

void BaseKnot::Expand(MicroMeterRect & umRect) const
{
	umRect.Expand(GetPos());
}

void BaseKnot::Check() const
{
	Nob::Check();
	m_inPipes .Check();
	m_outPipes.Check();
	Apply2AllInPipes ([this](Pipe const & p) { assert(p.GetEndKnotId  () == GetId()); });
	Apply2AllOutPipes([this](Pipe const & p) { assert(p.GetStartKnotId() == GetId()); });
}

void BaseKnot::Apply2AllConnectedPipes(PipeFunc const &f) const 
{ 
	Apply2AllInPipes(f); 
	Apply2AllOutPipes(f); 
}

bool BaseKnot::Apply2AllConnectedPipesB(function<bool(Pipe const&)> const & c) const
{ 
	return Apply2AllInPipesB(c) || Apply2AllOutPipesB(c); 
}

bool BaseKnot::IsPrecursorOf(Pipe const & pipeSucc) const 
{
	return Apply2AllOutPipesB([&pipeSucc](Pipe const & pipe) { return & pipe == & pipeSucc; }); 
}

bool BaseKnot::IsSuccessorOf(Pipe const & pipePred) const
{
	return Apply2AllInPipesB([&pipePred](Pipe const & pipe) { return & pipe == & pipePred; });
}

bool BaseKnot::IsPrecursorOf(BaseKnot const & b) const 
{
	return Apply2AllOutPipesB([&b](Pipe const & p){ return p.GetEndKnotPtr() == &b; });
}

bool BaseKnot::IsSuccessorOf(BaseKnot const & b) const
{
	return Apply2AllInPipesB([&b](Pipe const & p){ return p.GetStartKnotPtr() == &b; });
}

bool BaseKnot::IsDirectlyConnectedTo(BaseKnot const & b) const
{
	return IsSuccessorOf(b) || IsPrecursorOf(b);
}

bool BaseKnot::IsDirectlyConnectedTo(Pipe const & pipe) const
{
	return IsDirectlyConnectedTo(* static_cast<BaseKnot const *>(pipe.GetStartKnotPtr())) || 
		   IsDirectlyConnectedTo(* static_cast<BaseKnot const *>(pipe.GetEndKnotPtr  ()));
}

bool BaseKnot::Includes(MicroMeterPnt const & point) const
{
	return Distance(point, GetPos()) <= GetExtension();
}

MicroMeterRect BaseKnot::GetRect4Text() const
{
	return MicroMeterRect
	{
		GetPosX() - GetExtension(),      // left
		GetPosY() - GetExtension() / 2,  // top
		GetPosX() + GetExtension(),      // right
		GetPosY() + GetExtension()       // bottom
	};
}

BaseKnot const * Cast2BaseKnot(Nob const * nob)
{
	assert(! nob->IsPipe());
	assert(! nob->IsUndefined());
	return static_cast<BaseKnot const *>(nob);
}

BaseKnot * Cast2BaseKnot(Nob * nob)
{
	assert(! nob->IsPipe());
	assert(! nob->IsUndefined());
	return static_cast<BaseKnot *>(nob);
}

void BaseKnot::AppendMenuItems(AddMenuFunc const & add) const
{
	if (HasOutgoing())
		add(IDM_SELECT_SUBTREE);   
	Nob::AppendMenuItems(add);
}
