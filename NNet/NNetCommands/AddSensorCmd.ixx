// AddSensorCmd.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:AddSensorCmd;

import Signals;
import :NNetCommand;
import :NNetSound;

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
    {
        m_upSensor = make_unique<Sensor>(umCircle, m_pNMWI->GetUPNobsC()); 
        m_upSignal = make_unique<NNetSignal>(m_upSensor.get());
    }

    void Do() final
    {
        m_pNMWI->InsertTrack(m_trackNr);
        m_signalNr = m_pNMWI->AddSignal(move(m_upSignal), m_trackNr);
        m_sensorId = m_pNMWI->AddSensor(move(m_upSensor));                                         
        PlaySound(SNAP_IN_SOUND);
    };

    void Undo() final
    {
	   m_upSignal = m_pNMWI->RemoveSignal(SignalId(m_trackNr, m_signalNr));
       m_upSensor = m_pNMWI->RemoveSensor(m_sensorId);                                         
       m_pNMWI->DeleteTrack(m_trackNr);
       PlaySound(DISAPPEAR_SOUND);
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
			TraceStream() << NAME << umCircle << trackNr << endl;
		PushCommand(make_unique<AddSensorCmd>(umCircle, trackNr));
	}

private:

	inline static const wstring NAME { L"AddSensorSignal" };

	inline static struct myWrapper : public Wrapper
	{
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
		{
			MicroMeterCircle const umCircle { ScrReadMicroMeterCircle(script) };
			TrackNr          const trackNr  { ScrReadTrackNr(script) };
			AddSensorCmd::Push(umCircle, trackNr);
		}
    } m_wrapper { NAME };

    MicroMeterCircle const m_umCircle;
    TrackNr          const m_trackNr;
	SignalNr               m_signalNr;
    SensorId               m_sensorId;
	unique_ptr<Signal>     m_upSignal;
	unique_ptr<Sensor>     m_upSensor;
};
