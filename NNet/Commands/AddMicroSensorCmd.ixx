// AddMicroSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:AddMicroSensorCmd;

import :NNetCommand;

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
		PlaySound(L"SNAP_IN_SOUND");
	};

	void Undo() final
	{
		m_pNMWI->GetMonitorData().DeleteSignal(m_signalId);
		m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
		PlaySound(L"DISAPPEAR_SOUND");
	};

	static void Push
	(
		NobId   const nobId,
		TrackNr const trackNr
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << trackNr << endl;
		PushCommand(make_unique<AddMicroSensorCmd>(nobId, trackNr));
	}

private:

	inline static const wstring NAME { L"AddMicroSensor" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId   const nobId   { ScrReadNobId(script) };
			TrackNr const trackNr { ScrReadTrackNr(script) };
			AddMicroSensorCmd::Push(nobId, trackNr);
		}
	} m_wrapper { NAME };

	NobId    const m_nobId;
	TrackNr  const m_trackNr;
	SignalId       m_signalId {};
};
