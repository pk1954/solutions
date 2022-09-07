// AddSensorSignalCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"

export module AddSensorSignalCmd;

import Types;
import TrackNr;
import SignalId;
import NNetCommand;
import SignalFactory;
import MonitorData;

export class AddSensorSignalCmd : public NNetCommand
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
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
    };

private:

    MicroMeterCircle const m_umCircle;
    TrackNr          const m_trackNr;
    SignalId               m_signalId{};
};
