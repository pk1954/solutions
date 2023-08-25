// IoLine.cpp 
//
// NNetModel

module;

#include <cassert>
#include <iostream>

module NNetModel:IoLine;

import Types;
import :IoConnector;
import :PipeList;
import :Pipe;

using std::wcout;
using std::endl;

void IoLine::Dump() const
{
	Nob::Dump();
	wcout << endl;
}

void IoLine::Check() const
{
	PosNob::Check();
}

MicroMeterPnt IoLine::GetDirVector() const
{
	return Radian2Vector(GetDir()).ScaledTo(GetExtension());
}

Radian IoLine::GetDir() const
{
	return HasParentNob()
		? Cast2IoConnector(m_pIoConnector)->GetDir()
		: Vector2Radian(GetPipeC()->GetVector());
};

void IoLine::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}

void IoLine::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	IoLine const& src { static_cast<IoLine const&>(nobSrc) };
	SetPipe(static_cast<Pipe*>(f(src.GetPipeC())));
}

void IoLine::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	MicroMeterPnt umPos { GetPos() };
	umPos.Rotate(umPntPivot, radDelta);
	SetPos(umPos);
	if (m_pIoConnector)
		static_cast<IoConnector*>(m_pIoConnector)->DirectionDirty();
}

void IoLine::Recalc()
{
	m_pPipe->PosChanged();
	if (m_pIoConnector)
		static_cast<IoConnector*>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SelectAllConnected(bool const bFirst)
{
	if (!IsSelected() || bFirst)
	{
		Nob::Select(true);
		m_pPipe->SelectAllConnected(false);
		if (HasParentNob())
			GetParentNob()->SelectAllConnected(false);
	}
}

void IoLine::DirectionDirty()
{
	if (HasParentNob())
		m_pIoConnector->DirectionDirty();
}
