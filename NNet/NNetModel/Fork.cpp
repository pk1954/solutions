// Fork.cpp 
//
// NNetModel

module;

#include <cassert>
#include "Resource.h"

module NNetModel:Fork;

import DrawContext;
import Types;

Fork::Fork(MicroMeterPnt const center)
  : PosNob(NobType::Value::fork),
	m_circle(center, KNOT_WIDTH)
{}

Fork::Fork(Fork const & rhs)
	: PosNob(NobType::Value::fork)
{
	PosNob::operator=(rhs);

	m_circle    = rhs.m_circle;
	m_pPipeIn   = rhs.m_pPipeIn;
	m_pPipeOut1 = rhs.m_pPipeOut1;
	m_pPipeOut2 = rhs.m_pPipeOut2;
}

void Fork::Check() const
{
	PosNob::Check();
	m_pPipeIn->Check();
	m_pPipeOut1->Check();
	m_pPipeOut2->Check();
	assert(m_pPipeIn  ->GetEndKnotId()   == GetId());
	assert(m_pPipeOut1->GetStartKnotId() == GetId());
	assert(m_pPipeOut2->GetStartKnotId() == GetId());
}

Pipe* Fork::GetOtherOutgoing(Pipe* pPipe)
{
	if (pPipe == m_pPipeOut1)
		return m_pPipeOut2;
	else if (pPipe == m_pPipeOut2)
		return m_pPipeOut1;
	else
		assert(false);
	return nullptr;
}

void Fork::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}

void Fork::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Fork const& src { *Cast2Fork(&nobSrc) };
	m_pPipeIn   = static_cast<Pipe*>(f(src.m_pPipeIn  ));
	m_pPipeOut1 = static_cast<Pipe*>(f(src.m_pPipeOut1));
	m_pPipeOut2 = static_cast<Pipe*>(f(src.m_pPipeOut2));
}

void Fork::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	m_circle.Rotate(umPntPivot, radDelta);
}

void Fork::ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd)
{ 
	assert(pDel == m_pPipeIn);
	m_pPipeIn = pAdd;
}

void Fork::ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) 
{ 
	if (pDel == m_pPipeOut1)
		m_pPipeOut1 = pAdd;
	else if (pDel == m_pPipeOut2)
		m_pPipeOut2 = pAdd;
	else
		assert(false);
}

void Fork::Apply2AllInPipes(PipeFunc const& f)
{
	f(*m_pPipeIn);
}

void Fork::Apply2AllOutPipes(PipeFunc const& f)
{
	f(*m_pPipeOut1);
	f(*m_pPipeOut2);
}

void Fork::Apply2AllInPipesC(PipeFuncC const& f) const
{
	f(*m_pPipeIn);
}

void Fork::Apply2AllOutPipesC(PipeFuncC const& f) const
{
	f(*m_pPipeOut1);
	f(*m_pPipeOut2);
}

bool Fork::Apply2AllInPipesB(PipeCrit const& c) const
{ 
	return c(*m_pPipeIn); 
}

bool Fork::Apply2AllOutPipesB(PipeCrit const& c) const 
{ 
	return c(*m_pPipeOut1) || c(*m_pPipeOut2);
}

void Fork::SetPos(MicroMeterPnt const& newPos)
{
	m_circle.SetPos(newPos);
	m_pPipeIn  ->PosChanged();
	m_pPipeOut1->PosChanged();
	m_pPipeOut2->PosChanged();
}

void Fork::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	FillExternalCircle(context, type);
}

void Fork::DrawInterior(DrawContext const& context, tHighlight const type) const
{
	FillInternalCircle(context, type);
}

void Fork::SetAllIncoming(PosNob& src)
{
	m_pPipeIn = Cast2Fork(&src)->m_pPipeIn;
}

void Fork::SetAllOutgoing(PosNob& src)
{
	Fork* pForkSrc { Cast2Fork(&src) };
	m_pPipeOut1 = pForkSrc->m_pPipeOut1;
	m_pPipeOut2 = pForkSrc->m_pPipeOut2;
}

void Fork::Reconnect()
{
	m_pPipeIn->SetEndPnt(this);
	m_pPipeIn->PosChanged();
	m_pPipeOut1->SetStartPnt(this);
	m_pPipeOut1->PosChanged();
	m_pPipeOut2->SetStartPnt(this);
	m_pPipeOut1->PosChanged();
};

void Fork::AddIncoming(Pipe * pPipe)
{
	m_pPipeIn = pPipe;
}

void Fork::AddOutgoing(Pipe * pPipe)
{
	assert(m_pPipeOut2 == nullptr);
	if (m_pPipeOut1)
		m_pPipeOut2 = pPipe;
	else
		m_pPipeOut1 = pPipe;
}

void Fork::AppendMenuItems(AddMenuFunc const& add) const
{
	Nob::AppendMenuItems(add);
	add(IDD_DELETE_NOB);
}
