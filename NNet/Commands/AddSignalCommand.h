// AddSignalCommand.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "Command.h"

class NNetModelWriterInterface;

class AddSignalCommand: public Command
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

    virtual void Do( NNetModelWriterInterface & nmwi) 
    {
        MonitorData & monitorData { nmwi.GetMonitorData() };
        monitorData.InsertTrack( TrackNr(0) );
        m_signalId = SignalId(m_trackNr, monitorData.AddSignal(m_trackNr, m_umCircle));
    };

    virtual void Undo( NNetModelWriterInterface & nmwi )
    { 
        nmwi.GetMonitorData().DeleteSignal(m_signalId);
    };

private:

    MicroMeterCircle const m_umCircle;
    TrackNr          const m_trackNr;
    SignalId               m_signalId {};
};
