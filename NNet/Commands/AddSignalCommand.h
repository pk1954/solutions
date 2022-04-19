// AddSignalCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "NNetCommand.h"

class NNetModelWriterInterface;

class AddSignalCommand: public NNetCommand
{
public:

    AddSignalCommand
    (
        MicroMeterCircle const & umCircle,
        TrackNr          const   trackNr
   )
      : m_umCircle(umCircle),
        m_trackNr(trackNr)
    {}

    void Do() final 
    {
        MonitorData & monitorData { m_pNMWI->GetMonitorData() };
        monitorData.InsertTrack(TrackNr(0));
        m_signalId = SignalId
        (
            m_trackNr, 
            monitorData.AddSignal(m_pNMWI->GetUPNobsC(), m_trackNr, m_umCircle)
        );
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
