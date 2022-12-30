// IoLine.cpp 
//
// NNetModel

module;

#include <iostream>

module NNetModel:IoLine;

import Types;
import :MicroMeterPosDir;
import :IoConnector;
import :PipeList;
import :Pipe;

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
	m_circle.Rotate(umPntPivot, radDelta);
	if (m_pIoConnector)
		static_cast<IoConnector*>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SetPos(MicroMeterPnt const& pos)
{
	m_circle.SetPos(pos);
	m_pPipe->PosChanged();
	if (m_pIoConnector)
		static_cast<IoConnector *>(m_pIoConnector)->DirectionDirty();
}

void IoLine::SetDir(Radian const r)
{ 
	m_radDirection = r;
	m_pIoConnector = nullptr;
}
