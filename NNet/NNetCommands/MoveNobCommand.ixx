// MoveNobCommand.ixx
//
// NNetCommands

module;

#include <cassert>

export module NNetCommands:MoveNobCommand;

import std;
import IoUtil;
import :NNetCommand;

export class MoveNobCommand : public NNetCommand
{
public:
	MoveNobCommand
	(
		Nob                 & nob, 
		MicroMeterPnt const & delta 
	)
	  : m_delta(delta),
		m_nob(nob)
	{ 
		assert(&nob);
	}

	void Do() final 
	{ 
		m_nob.MoveNob(m_delta);
	}

	void Undo() final
	{ 
		m_nob.MoveNob(-m_delta);
	}

	NobId GetAffectedNob() const final { return m_nob.GetId(); }

	bool CombineCommands(BaseCommand const & src) final
	{ 
		MoveNobCommand const & srcCmd { static_cast<MoveNobCommand const &>(src) };
		if (GetAffectedNob() != srcCmd.GetAffectedNob())
			return false;
		m_delta += srcCmd.m_delta;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& delta)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << delta << endl;
		PushCommand(make_unique<MoveNobCommand>(*m_pNMWI->GetNob(nobId), delta));
	}

private:

	inline static const wstring NAME { L"MoveNob" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId         const nobId   { ScrReadNobId(script) };
			MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
			MoveNobCommand::Push(nobId, umDelta);
		}
	} m_wrapper { NAME };

	MicroMeterPnt m_delta;
	Nob           & m_nob;
};
