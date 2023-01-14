// Knot.cpp 
//
// NNetModel

module;

#include <memory>
#include <cassert>
#include "Resource.h"

module NNetModel:Knot;

import DrawContext;
import Types;
import :NobType;
import :tHighlight;
import :InputLine;
import :OutputLine;
import :PosNob;
import :PipeList;
import :Pipe;

using std::unique_ptr;
using std::make_unique;

Knot::Knot(MicroMeterPnt const& center)
  : PosNob(NobType::Value::knot),
	m_circle(center, KNOT_WIDTH)
{}

Knot::Knot(PosNob const& posNob)
  : PosNob(NobType::Value::knot),
	m_circle(posNob.GetPos(), KNOT_WIDTH)
{
	SetId(posNob.GetId());
}

void Knot::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	FillExternalCircle(context, type);
}

void Knot::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	FillInternalCircle(context, type);
}

void Knot::Check() const
{
	PosNob::Check();
}

void Knot::Emphasize(bool const bOn, bool bDownStream) 
{ 
	Nob::Emphasize(bOn);
	if (bDownStream)
		m_pPipeOut->Emphasize(bOn, true);
	else
		m_pPipeIn->Emphasize(bOn, false);
}

void Knot::ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd)
{
	assert(pDel == m_pPipeIn);
	m_pPipeIn = pAdd;
}

void Knot::ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd)
{
	assert(pDel == m_pPipeOut);
	m_pPipeOut = pAdd;
}

void Knot::SetPosNoFix(MicroMeterPnt const& pos)
{
	m_circle.SetPos(pos);
}

void Knot::Recalc()
{
	m_pPipeIn->PosChanged();
	m_pPipeOut->PosChanged();
}

void Knot::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}

void Knot::Reconnect()
{
	m_pPipeIn->SetEndPnt(this);
	m_pPipeOut->SetStartPnt(this);
	m_pPipeIn->PosChanged();
	m_pPipeOut->PosChanged();
};

void Knot::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Knot const& src { static_cast<Knot const&>(nobSrc) };
	m_pPipeIn  = static_cast<Pipe*>(f(src.m_pPipeIn));
	m_pPipeOut = static_cast<Pipe*>(f(src.m_pPipeOut));
}

void Knot::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	m_circle.Rotate(umPntPivot, radDelta);
}

void Knot::SetAllIncoming(PosNob& src)
{
	m_pPipeIn = Cast2Knot(&src)->m_pPipeIn;
}

void Knot::SetAllOutgoing(PosNob& src)
{
	m_pPipeIn = Cast2Knot(&src)->m_pPipeIn;
}

void Knot::AppendMenuItems(AddMenuFunc const& add) const
{
	Nob::AppendMenuItems(add);
	add(IDD_DELETE_NOB);
}

Knot const* Cast2Knot(Nob const* pNob)
{
	assert(pNob);
	assert(pNob->IsKnot());
	return static_cast<Knot const*>(pNob);
}

Knot* Cast2Knot(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsKnot());
	return static_cast<Knot*>(pNob);
}
