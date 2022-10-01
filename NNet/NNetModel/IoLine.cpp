// IoLine.cpp 
//
// NNetModel

module;

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
	MicroMeterPnt umVector { MicroMeterPnt::ZERO_VAL() };

	Apply2AllConnectedPipes
	(
		[&umVector](Pipe const & pipe) { umVector += Normalize(pipe.GetVector()); } 
	);

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

MicroMeterPosDir IoLine::GetRawPosDir() const
{
	return MicroMeterPosDir(GetPos(), m_radDirection);
}

MicroMeterPosDir IoLine::GetPosDir() const
{
	return MicroMeterPosDir(GetPos(), Vector2Radian(GetDirVector()));
}

void IoLine::UnlockDirection() 
{
	SetDir(Radian::NULL_VAL());
}

void IoLine::LockDirection() 
{ 
	if (!IsDirLocked()) 
		SetDir(Vector2Radian(determineVector()));
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
	unsigned int nrLeft{ 0 };
	unsigned int nrRight{ 0 };
	for (auto pBaseKnot : list)
	{
		pBaseKnot->Apply2AllInPipes
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
		pBaseKnot->Apply2AllOutPipes
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
