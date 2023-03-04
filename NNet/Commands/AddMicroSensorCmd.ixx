// AddMicroSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module AddMicroSensorCmd;

import NNetCommand;

export class AddMicroSensorCmd : public NNetCommand
{
public:

	AddMicroSensorCmd
	(
		NobId   const nobId,
		TrackNr const trackNr
	)
      : m_nobId(nobId),
		m_trackNr(trackNr)
	{}

	void Do() final
	{
		m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
		m_signalId = SignalFactory::MakeMicroSensorSignal(m_nobId, m_trackNr, *m_pNMWI);
		m_pSound->Play(L"SNAP_IN_SOUND");
	};

	void Undo() final
	{
		m_pNMWI->GetMonitorData().DeleteSignal(m_signalId);
		m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
		m_pSound->Play(L"DISAPPEAR_SOUND");
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push
	(
		NobId   const nobId,
		TrackNr const trackNr
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId.GetValue() << L' ' << trackNr.GetValue() << endl;
		m_pStack->PushCommand(make_unique<AddMicroSensorCmd>(nobId, trackNr));
	}

private:

	inline static const wstring NAME { L"AddMicroSensor" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId   const nobId   { ScrReadNobId(script) };
			TrackNr const trackNr { ScrReadTrackNr(script) };
			AddMicroSensorCmd::Push(nobId, trackNr);
		}
	};

	NobId    const m_nobId;
	TrackNr  const m_trackNr;
	SignalId       m_signalId {};
};
