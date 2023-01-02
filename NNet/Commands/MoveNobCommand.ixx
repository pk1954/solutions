// MoveNobCommand.ixx
//
// Commands

module;

#include <cassert>
#include <string>
#include <memory>
#include <iostream>

export module MoveNobCommand;

import Types;
import Symtab;
import Script;
import Commands;
import NNetWrapperHelpers;
import NNetCommand;
import NNetModel;

using std::endl;
using std::wstring;
using std::make_unique;
using std::unique_ptr;

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

	bool CombineCommands(Command const & src) final
	{ 
		MoveNobCommand const & srcCmd { static_cast<MoveNobCommand const &>(src) };
		if (GetAffectedNob() != srcCmd.GetAffectedNob())
			return false;
		m_delta += srcCmd.m_delta;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& delta)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << delta << endl;
		m_pStack->PushCommand(make_unique<MoveNobCommand>(*m_pNMWI->GetNob(nobId), delta));
	}

private:

	inline static const wstring NAME { L"MoveNob" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId         const nobId   { ScrReadNobId(script) };
			MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
			MoveNobCommand::Push(nobId, umDelta);
		}
	};

	MicroMeterPnt m_delta;
	Nob           & m_nob;
};
