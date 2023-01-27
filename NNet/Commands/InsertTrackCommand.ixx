// InsertTrackCommand.ixx
//
// Commands

export module InsertTrackCommand;

import Types;
import NNetCommand;
import NNetModel;

export class InsertTrackCommand: public NNetCommand
{
public:

    InsertTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    void Do() final 
    {
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
        m_pSound->Play(L"SNAP_IN_SOUND");
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
        m_pSound->Play(L"DISAPPEAR_SOUND");
    };

private:

    TrackNr m_trackNr;
};
