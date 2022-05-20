// AddSensorSignalCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "NNetCommand.h"

class NNetModelWriterInterface;

class AddSensorSignalCmd: public NNetCommand
{
public:

    AddSensorSignalCmd
    (
        MicroMeterCircle const & umCircle,
        TrackNr          const   trackNr
   )
      : m_umCircle(umCircle),
        m_trackNr(trackNr)
    {}

    void Do() final 
    {
        MonitorData      & monitorData { m_pNMWI->GetMonitorData() };
        unique_ptr<Sensor> upSensor    { make_unique<Sensor>(m_umCircle, m_pNMWI->GetUPNobsC()) };  // create sensor
        monitorData.InsertTrack(m_trackNr);                                                         // create new track  
        SignalNr     const signalNr    { monitorData.AddSensorSignal(m_trackNr, *upSensor.get()) }; // add to monitor      
        m_signalId = SignalId(m_trackNr, signalNr);                                                 //  - creates signal & adds to track 
        m_pNMWI->GetSensorList().PushSensor(move(upSensor));                                        // add to sensor list
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().DeleteSignal(m_signalId);
    };

private:

    MicroMeterCircle const m_umCircle;
    TrackNr          const m_trackNr;
    SignalId               m_signalId {};
};
