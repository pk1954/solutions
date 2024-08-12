// RotateNobCommand.ixx
//
// NNetCommands

export module NNetCommands:RotateNobCommand;

import std;
import IoUtil;
import NNetModel;
import :RotationCommand;

export class RotateNobCommand : public RotationCommand
{
public:
	RotateNobCommand
	(
		NobId         const   id,
		MicroMeterPnt const & umPntOld,
		MicroMeterPnt const & umPntNew
	)
		: m_nob(*m_pNMWI->GetNob(id))
	{
		SetPivotPnt(m_nob.GetPos(), umPntOld, umPntNew);
	}

	void Do()   final { DoRotate(m_nob); }
	void Undo() final { UndoRotate(m_nob); }

	NobId GetAffectedNob() const final { return m_nob.GetId(); }

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push
	(
		NobId         const  id,
		MicroMeterPnt const& umPntOld,
		MicroMeterPnt const& umPntNew
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << umPntOld << umPntNew << endl;
		PushCommand(make_unique<RotateNobCommand>(id, umPntOld, umPntNew));
	}

private:

	inline static const wstring NAME { L"RotateNob" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId         const idNob    { ScrReadNobId(script) };
			MicroMeterPnt const umPntOld { ScrReadMicroMeterPnt(script) };
			MicroMeterPnt const umPntNew { ScrReadMicroMeterPnt(script) };
			RotateNobCommand::Push(idNob, umPntOld, umPntNew);
		}
	} m_wrapper { NAME };

	Nob & m_nob;
};
