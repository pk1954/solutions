// MoveSignalCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:MoveSignalCmd;

import IoUtil;
import :NNetCommand;

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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(SignalId const& id, TrackNr const trackNr)
	{
		//if (IsTraceOn())          // TODO: fix
		//	TraceStream() << NAME << id << SPACE << trackNr.GetValue() << endl;
		PushCommand(make_unique<MoveSignalCmd>(id, trackNr));
	}

private:

	inline static const wstring NAME { L"MoveSignal" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			SignalId const id      { ScrReadSignalId(script) };
			TrackNr  const trackNr { ScrReadTrackNr(script) };
			MoveSignalCmd::Push(id, trackNr);
		}
	} m_wrapper;

	SignalId const m_signalIdOld;
	TrackNr  const m_trackNrNew;
	TrackNr        m_trackNrOld;
	SignalNr       m_signalNrNew;
};
