// RotationCommand.ixx
//
// Commands

export module NNetCommands:RotationCommand;

import Types;
import :NNetCommand;
import NNetModel;

export class RotationCommand : public NNetCommand
{
public:

	bool CombineCommands(BaseCommand const& src) override
	{
		RotationCommand const& srcCmd{ static_cast<RotationCommand const&>(src) };
		m_radDelta += srcCmd.m_radDelta;
		return true;
	}

protected:

	void SetPivotPnt
	(
		MicroMeterPnt const& umPnt,
		MicroMeterPnt const& umPntOld,
		MicroMeterPnt const& umPntNew
	)
	{
		Radian const radOld{ Vector2Radian(umPntOld - umPnt) };
		Radian const radNew{ Vector2Radian(umPntNew - umPnt) };
		m_radDelta = radNew - radOld;
		m_umPntPivot = umPnt;
	}

	void DoRotate(Nob& nob) const
	{
		nob.RotateNob(m_umPntPivot, m_radDelta);
	}

	void UndoRotate(Nob& nob) const
	{
		nob.RotateNob(m_umPntPivot, -m_radDelta);
	}

	void DoRotateSensor(Sensor& s) const
	{
		s.RotateSensor(m_umPntPivot, m_radDelta);
	}

	void UndoRotateSensor(Sensor& s) const
	{
		s.RotateSensor(m_umPntPivot, -m_radDelta);
	}

private:
	Radian        m_radDelta;
	MicroMeterPnt m_umPntPivot;
};
