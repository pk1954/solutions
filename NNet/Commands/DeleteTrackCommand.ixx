// DeleteTrackCommand.ixx
//
// Commands

module;

#include <cassert>

export module DeleteTrackCommand;

import Types;
import NNetCommand;
import NNetModel;

export class DeleteTrackCommand: public NNetCommand
{
public:

    explicit DeleteTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    void Do() final 
    {
        MonitorData & monitorData { m_pNMWI->GetMonitorData() };
        assert(monitorData.IsEmptyTrack(m_trackNr));
        monitorData.DeleteTrack(m_trackNr);
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
    };

private:

    TrackNr m_trackNr;
};
