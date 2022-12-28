// IoLine.cpp 
//
// NNetModel

module;

#include <iostream>
#include <vector>

module NNetModel:IoLine;

import Types;
import :MicroMeterPosDir;
import :PipeList;
import :Pipe;

using std::vector;

MicroMeterPnt IoLine::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPnt IoLine::determineVector() const
{
	MicroMeterPnt umVector { Normalize(GetPipeC()->GetVector()) };

	if (umVector.IsZero())
		umVector = MicroMeterPnt(0.0_MicroMeter, 1.0_MicroMeter);

	return umVector;
}

MicroMeterPnt IoLine::GetDirVector() const
{
	MicroMeterPnt umVector 
	{ 
		m_radDirection.IsNull() 
		? determineVector() 
		: Radian2Vector(m_radDirection) 
	};
	return umVector.ScaledTo(GetExtension());
}

Radian IoLine::GetDir() const 
{ 
	return (IsDirLocked())
		? m_radDirection
		: Vector2Radian(determineVector());
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

MicroMeterPosDir IoLine::GetRawPosDir() const
{
	return MicroMeterPosDir(GetPos(), m_radDirection);
}

MicroMeterPosDir IoLine::GetPosDir() const
{
	return MicroMeterPosDir(GetPos(), Vector2Radian(GetDirVector()));
}

void IoLine::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	m_circle.Rotate(umPntPivot, radDelta);
}

bool IoLine::UnlockDirection()
{
	std::wcout << L"UnlockDirection" << std::endl;
	bool bRes { IsDirLocked() };
	SetDir(Radian::NULL_VAL());
	return bRes;
}

bool IoLine::LockDirection() 
{ 
	std::wcout << L"LockDirection" << std::endl;
	bool bRes { IsDirLocked() };
	if (!bRes)
		SetDir(Vector2Radian(determineVector()));
	return bRes;
}

void IoLine::SetPos(MicroMeterPnt const& pos)
{
	m_circle.SetPos(pos);
	m_pPipe->PosChanged();
}

void IoLine::SetDir(Radian const r)
{ 
	m_radDirection = r; 
}

bool IoLine::IsDirLocked() const
{
	return m_radDirection.IsNotNull();
}

MicroMeterPnt CalcOrthoVector(vector<IoLine*> const& list, MicroMeterLine const& line)
{
	unsigned int nrLeft  { 0 };
	unsigned int nrRight { 0 };
	for (auto pPosNob : list)
	{
		pPosNob->Apply2AllInPipesC
		(
			[&line, &nrLeft, &nrRight](Pipe const& pipe)
			{
				MicroMeterPnt pnt{ pipe.GetStartPoint() };
				if (PointToLine(line, pnt) < 0.0_MicroMeter)
					++nrLeft;
				else
					++nrRight;
			}
		);
		pPosNob->Apply2AllOutPipesC
		(
			[&line, &nrLeft, &nrRight](Pipe const& pipe)
			{
				MicroMeterPnt pnt{ pipe.GetEndPoint() };
				if (PointToLine(line, pnt) < 0.0_MicroMeter)
					++nrRight;
				else
					++nrLeft;
			}
		);
	}

	MicroMeterPnt orthoVector{ line.OrthoVector() };
	if (nrRight < nrLeft)
		orthoVector = -orthoVector;
	return orthoVector;
}
