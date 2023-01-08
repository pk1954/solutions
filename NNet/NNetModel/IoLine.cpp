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
		assert(m_radDirection.IsNull());  // connected to IoConnector
	}
	else
	{
		if (m_radDirection.IsNull())
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
	return m_radDirection.IsNull()
		   ? getDirection()
		   : m_radDirection;
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
	m_umPosition.Rotate(umPntPivot, radDelta);
	if (m_pIoConnector)
		static_cast<IoConnector*>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SetPos(MicroMeterPnt const& pos)
{
	m_umPosition = pos;
	m_pPipe->PosChanged();
	if (m_pIoConnector)
		static_cast<IoConnector *>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SetDir(Radian const r)
{ 
	m_radDirection = r;
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
