// IoLine.cpp 
//
// NNetModel

module;

#include <cassert>
#include <iostream>

module NNetModel:IoLine;

import Types;
import :MicroMeterPosDir;
import :IoConnector;
import :PipeList;
import :Pipe;

void IoLine::Check() const
{
	PosNob::Check();
	if (HasParentNob())
	{
		assert(m_posDir.GetDir().IsNull());  // connected to IoConnector
	}
	else
	{
		if (m_posDir.GetDir().IsNull())
		{
			// standard case, direction is pipe vector
		}
		else
		{
			// locked, direction explicitely defined
		}
	}
}

Radian IoLine::getDirection() const
{
	return HasParentNob()
		? Cast2IoConnector(m_pIoConnector)->GetDir()
		: Vector2Radian(GetPipeC()->GetVector());
}

MicroMeterPnt IoLine::GetDirVector() const
{
	return Radian2Vector(GetDir()).ScaledTo(GetExtension());
}

Radian IoLine::GetDir() const 
{ 
	return m_posDir.GetDir().IsNull()
		   ? getDirection()
		   : m_posDir.GetDir();
};

//void IoLine::MoveNob(MicroMeterPnt const& delta)
//{
//	m_posDir.SetPos(GetPos() + delta);
//}

void IoLine::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	IoLine const& src { static_cast<IoLine const&>(nobSrc) };
	SetPipe(static_cast<Pipe*>(f(src.GetPipeC())));
}

void IoLine::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	MicroMeterPnt umPos { GetPos() };
	umPos.Rotate(umPntPivot, radDelta);
	m_posDir.SetPos(umPos);
	if (m_pIoConnector)
		static_cast<IoConnector*>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SetPosNoFix(MicroMeterPnt const& pos)
{
	m_posDir.SetPos(pos);
}

void IoLine::Recalc()
{
	m_pPipe->PosChanged();
	if (m_pIoConnector)
		static_cast<IoConnector*>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SetDir(Radian const r)
{ 
	m_posDir.SetDir(r);
	m_pIoConnector = nullptr;
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
