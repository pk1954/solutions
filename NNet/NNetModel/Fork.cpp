// Fork.cpp 
//
// NNetModel

module;

#include <cassert>

module NNetModel:Fork;

import DrawContext;
import Types;

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

void Fork::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}

void Fork::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Fork const& src { static_cast<Fork const&>(nobSrc) };
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

void Fork::Apply2AllInPipes(PipeFunc const& f) const 
{ 
	f(*m_pPipeIn);
}

void Fork::Apply2AllOutPipes(PipeFunc const& f) const 
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
	m_pPipeIn  ->Recalc();
	m_pPipeOut1->Recalc();
	m_pPipeOut2->Recalc();
}

void Fork::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	FillExternalCircle(context, type);
}

void Fork::DrawInterior(DrawContext const& context, tHighlight const type) const
{
	FillInternalCircle(context, type);
}
