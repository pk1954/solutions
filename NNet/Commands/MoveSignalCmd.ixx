// MoveSignalCmd.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module MoveSignalCmd;

import Symtab;
import Script;
import Commands;
import NNetWrapperHelpers;
import NNetCommand;
import NNetModel;

using std::endl;
using std::wstring;
using std::make_unique;

export class MoveSignalCmd : public NNetCommand
{
public:
	MoveSignalCmd
	(
		SignalId const & id,
		TrackNr  const   trackNr
	)
      : m_signalIdOld(id),
		m_trackNrNew(trackNr)
	{}

	void Do() final 
	{ 
		m_trackNrOld  = m_signalIdOld.GetTrackNr();
		m_signalNrNew = m_pNMWI->GetMonitorData().MoveSignal(m_signalIdOld, m_trackNrNew);
	}

	void Undo() final 
	{
		m_pNMWI->GetMonitorData().MoveSignal(SignalId(m_trackNrNew, m_signalNrNew), m_trackNrOld);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(SignalId const& id, TrackNr const trackNr)
	{
		if (IsTraceOn())
			TraceStream() << NAME << L" " << id << L" " << trackNr << endl;
		m_pStack->PushCommand(make_unique<MoveSignalCmd>(id, trackNr));
	}

private:

	inline static const wstring NAME { L"MoveSignal" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			SignalId const id      { ScrReadSignalId(script) };
			TrackNr  const trackNr { ScrReadTrackNr(script) };
			MoveSignalCmd::Push(id, trackNr);
		}
	};

	SignalId const m_signalIdOld;
	TrackNr  const m_trackNrNew;
	TrackNr        m_trackNrOld;
	SignalNr       m_signalNrNew;
};
