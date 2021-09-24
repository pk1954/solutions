// InsertTrackCommand.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "NNetCommand.h"

class InsertTrackCommand: public NNetCommand
{
public:

    InsertTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    virtual void Do() 
    {
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
    };

    virtual void Undo()
    { 
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
    };

private:

    TrackNr m_trackNr;
};
