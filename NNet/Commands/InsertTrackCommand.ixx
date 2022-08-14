// InsertTrackCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "NNetCommand.h"

export module InsertTrackCommand;

import MoreTypes;

export class InsertTrackCommand: public NNetCommand
{
public:

    InsertTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    void Do() final 
    {
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
    };

private:

    TrackNr m_trackNr;
};
