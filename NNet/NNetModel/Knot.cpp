// Knot.cpp 
//
// NNetModel

module;

#include <cassert>
#include "Resource.h"

module NNetModel:Knot;

import DrawContext;
import Types;
import :NobType;
import :tHighlight;
import :PosNob;
import :PipeList;
import :Pipe;

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

void Knot::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}

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
