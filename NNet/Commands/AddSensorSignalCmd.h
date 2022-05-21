// AddSensorSignalCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SignalFactory.h"
#include "MonitorData.h"
#include "NNetCommand.h"

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
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
        m_signalId = SignalFactory::MakeSensorSignal(m_umCircle, m_trackNr, *m_pNMWI);
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
