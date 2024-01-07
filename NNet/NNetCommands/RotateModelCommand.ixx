// RotateModelCommand.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:RotateModelCommand;

import IoUtil;
import NNetModel;
import :RotationCommand;

export class RotateModelCommand : public RotationCommand
{
public:
	RotateModelCommand
	(
		MicroMeterPnt const & umPntOld,
		MicroMeterPnt const & umPntNew
	)
	{
		SetPivotPnt
		(
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const& n) { return true; }),
			umPntOld,
			umPntNew
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2All<PosNob>([this](PosNob& b) { DoRotate(b); });
		m_pNMWI->GetSensorList().Apply2All([this](Sensor* p) { DoRotateSensor(*p); });
	}

	void Undo() final
	{
		m_pNMWI->GetUPNobs().Apply2All<PosNob>([this](PosNob& b) { UndoRotate(b); });
		m_pNMWI->GetSensorList().Apply2All([this](Sensor* p) { UndoRotateSensor(*p); });
	}
	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push
	(
		MicroMeterPnt const& umPntOld,
		MicroMeterPnt const& umPntNew
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << umPntOld << umPntNew << endl;
		PushCommand(make_unique<RotateModelCommand>(umPntOld, umPntNew));
	}

private:

	inline static const wstring NAME { L"RotateModel" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			MicroMeterPnt const umPntOld { ScrReadMicroMeterPnt(script) };
			MicroMeterPnt const umPntNew { ScrReadMicroMeterPnt(script) };
			RotateModelCommand::Push(umPntOld, umPntNew);
		}
	} m_wrapper { NAME };
};
