// AddSensorSignalCmd.ixx
//
// Commands

export module AddSensorSignalCmd;

import Types;
import NNetCommand;
import NNetModel;

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
        m_pSound->Play(L"SNAP_IN_SOUND");
    };

    void Undo() final
    {
        m_pNMWI->GetMonitorData().DeleteSignal(m_signalId);
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
        m_pSound->Play(L"DISAPPEAR_SOUND");
    };

private:

    MicroMeterCircle const m_umCircle;
    TrackNr          const m_trackNr;
    SignalId               m_signalId{};
};
