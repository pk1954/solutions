// InsertTrackCommand.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "Command.h"

class InsertTrackCommand: public Command
{
public:

    InsertTrackCommand( TrackNr const trackNr )
        : m_trackNr(trackNr)
    {}

    virtual void Do( NNetModelWriterInterface & nmwi ) 
    {
        nmwi.GetMonitorData().InsertTrack(m_trackNr);
    };

    virtual void Undo( NNetModelWriterInterface & nmwi )
    { 
        nmwi.GetMonitorData().DeleteTrack(m_trackNr);
    };

private:

    TrackNr m_trackNr;
};
