// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "Resource.h"
#include "Geometry.h"
#include "DrawContext.h"
#include "BaseKnot.h"

using std::wcout;
using std::endl;
using std::find;
using std::begin;
using std::end;

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
	return (this->Nob::operator==(rhs))                   &&
	       GetPos      ().IsCloseTo(baseKnotRhs.GetPos()) &&
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

void BaseKnot::AddIncoming(BaseKnot const & src) 
{ 
	src.Apply2AllInPipes ([this](Pipe & pipe) { AddIncoming(& pipe); });
}

void BaseKnot::AddOutgoing(BaseKnot const & src) 
{ 
	src.Apply2AllOutPipes([this](Pipe & pipe) { AddOutgoing(& pipe); });
}

void BaseKnot::SetConnections(BaseKnot const & src) 
{ 
	SetIncoming(src);
	SetOutgoing(src);
	Reconnect();
}

void BaseKnot::Remove(Pipe * const p) 
{ 
	if (m_outPipes.TryRemove(p)) return;
	if (m_inPipes .TryRemove(p)) return;
	assert(false);
}

void BaseKnot::ClearConnections()
{
	m_inPipes .Clear();
	m_outPipes.Clear();
}

void BaseKnot::Reconnect() 
{ 
	m_inPipes .Apply2All([this](Pipe & pipe) { pipe.SetEndKnot  (this); });
	m_outPipes.Apply2All([this](Pipe & pipe) { pipe.SetStartKnot(this); });
}

void BaseKnot::Link(Nob const & nobSrc,	Nob2NobFunc const & f)
{
	BaseKnot const & src { static_cast<BaseKnot const &>(nobSrc) };
	ClearConnections();
	src.Apply2AllOutPipes([this,f](Pipe const &p){AddOutgoing(static_cast<Pipe *>(f(&p)));});
	src.Apply2AllInPipes ([this,f](Pipe const &p){AddIncoming(static_cast<Pipe *>(f(&p)));});
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

bool BaseKnot::Apply2AllConnectedPipesB(PipeCrit const &c) const 
{ 
	return Apply2AllInPipesB(c) || Apply2AllOutPipesB(c); 
}

void BaseKnot::Prepare()
{
	m_mVinputBuffer.Set2Zero();
	Apply2AllInPipes([this](Pipe const & pipe) { m_mVinputBuffer += pipe.GetNextOutput(); }); // slow !!
}

bool BaseKnot::IsPrecursorOf(Pipe const & pipeSucc) const 
{
	return m_outPipes.Apply2AllB([&pipeSucc](Pipe const & pipe) { return & pipe == & pipeSucc; }); 
}

bool BaseKnot::IsSuccessorOf(Pipe const & pipePred) const
{
	return m_inPipes.Apply2AllB([&pipePred](Pipe const & pipe) { return & pipe == & pipePred; });
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

void BaseKnot::drawCircle
(
	DrawContext      const & context, 
	D2D1::ColorF     const   colF, 
	MicroMeterCircle const   umCircle
) const
{
	context.FillCircle(umCircle, colF);
}

void BaseKnot::drawCircle
(
	DrawContext  const & context, 
	D2D1::ColorF const   colF, 
	MicroMeter   const   umWidth
) const
{
	context.FillCircle(MicroMeterCircle(GetPos(), umWidth), colF);
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
	if (GetNrOfConnections() > 1) 
		add(IDD_DISC_BASEKNOT);
	if (HasOutgoing())
		add(IDM_SELECT_SUBTREE);   
	if (IsAnyNeuron())
	{
		add(IDD_TRIGGER_SOUND_DLG);
		add(IDD_STOP_ON_TRIGGER);      
	}
	Nob::AppendMenuItems(add);
}
