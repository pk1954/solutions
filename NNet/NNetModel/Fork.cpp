// Fork.cpp 
//
// NNetModel

module;

module NNetModel:Fork;

import DrawContext;
import Types;

bool Fork::IsIncludedIn(MicroMeterRect const& umRect) const
{
	return umRect.Includes(GetPos());
}

void Fork::Expand(MicroMeterRect& umRect) const
{
	umRect.Expand(GetPos());
}

bool Fork::Includes(MicroMeterPnt const& point) const
{
	return Distance(point, GetPos()) <= GetExtension();
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

void Fork::SetPos(MicroMeterPnt const& newPos)
{
	m_circle.SetPos(newPos);
	m_pPipeIn  ->Recalc();
	m_pPipeOut1->Recalc();
	m_pPipeOut2->Recalc();
}

void Fork::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}
