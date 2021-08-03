// DeleteTrackCommand.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "Command.h"

class DeleteTrackCommand: public Command
{
public:

    DeleteTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    virtual void Do(NNetModelWriterInterface & nmwi) 
    {
        MonitorData & monitorData { nmwi.GetMonitorData() };
        assert(monitorData.IsEmptyTrack(m_trackNr));
        monitorData.DeleteTrack(m_trackNr);
    };

    virtual void Undo(NNetModelWriterInterface & nmwi)
    { 
        nmwi.GetMonitorData().InsertTrack(m_trackNr);
    };

private:

    TrackNr m_trackNr;
};
