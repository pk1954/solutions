// RotateSelectionCommand.ixx
//
// NNetCommands

export module NNetCommands:RotateSelectionCommand;

import std;
import IoUtil;
import NNetModel;
import :RotationCommand;

export class RotateSelectionCommand : public RotationCommand
{
public:
	RotateSelectionCommand
	(
		MicroMeterPnt const& umPntOld,
		MicroMeterPnt const& umPntNew
	)
	{
		SetPivotPnt
		(
			m_pNMWI->GetUPNobs().CenterOfGravity([](Nob const& n) { return n.IsSelected(); }),
			umPntOld,
			umPntNew
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>([this](PosNob& b) { DoRotate(b); });
	}

	void Undo() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>([this](PosNob& b) { UndoRotate(b); });
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
		PushCommand(make_unique<RotateSelectionCommand>(umPntOld, umPntNew));
	}

private:

	inline static const wstring NAME { L"RotateSelection" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			MicroMeterPnt const umPntOld { ScrReadMicroMeterPnt(script) };
			MicroMeterPnt const umPntNew { ScrReadMicroMeterPnt(script) };
			RotateSelectionCommand::Push(umPntOld, umPntNew);
		}
	} m_wrapper { NAME };

};
