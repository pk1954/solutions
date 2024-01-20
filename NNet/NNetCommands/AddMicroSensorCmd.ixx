// AddMicroSensorCmd.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:AddMicroSensorCmd;

import Signals;
import :NNetCommand;

using std::move;

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
	{
		m_upMicroSensor = make_unique<MicroSensor>(m_pNMWI->GetConstNob(nobId));
	    m_upSignal      = make_unique<NNetSignal>(m_upMicroSensor.get());   
	}

	void Do() final
	{
		m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
		m_signalNr = m_pNMWI->AddSignal(move(m_upSignal), m_trackNr);
		m_pNMWI->AddMicroSensor(move(m_upMicroSensor));
		PlaySound(L"SNAP_IN_SOUND");
	};

	void Undo() final
	{
		m_upMicroSensor = m_pNMWI->RemoveMicroSensor(m_nobId);
		m_upSignal      = m_pNMWI->RemoveSignal(SignalId(m_trackNr, m_signalNr));
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

	NobId             const m_nobId;
	TrackNr           const m_trackNr;
	SignalNr                m_signalNr;
	unique_ptr<Signal>      m_upSignal;
	unique_ptr<MicroSensor> m_upMicroSensor;
};
