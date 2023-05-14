// AddSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:AddSensorCmd;

import IoUtil;
import :NNetCommand;

export class AddSensorCmd : public NNetCommand
{
public:

    AddSensorCmd
    (
        MicroMeterCircle const & umCircle,
        TrackNr          const   trackNr
    )
      : m_umCircle(umCircle),
        m_trackNr(trackNr)
    {}

    void Do() final
    {
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
        m_signalId = SignalFactory::MakeSensorSignal(m_umCircle, m_trackNr, *m_pNMWI);
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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push
	(
		MicroMeterCircle const& umCircle,
		TrackNr          const  trackNr
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << umCircle << SPACE << trackNr.GetValue() << endl;
		PushCommand(make_unique<AddSensorCmd>(umCircle, trackNr));
	}

private:

	inline static const wstring NAME { L"AddSensorSignal" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			MicroMeterCircle const umCircle { ScrReadMicroMeterCircle(script) };
			TrackNr          const trackNr  { ScrReadTrackNr(script) };
			AddSensorCmd::Push(umCircle, trackNr);
		}
    } m_wrapper;

    MicroMeterCircle const m_umCircle;
    TrackNr          const m_trackNr;
    SignalId               m_signalId{};
};
