// InsertTrackCommand.h
//
// Commands

#pragma once

#include "MonitorData.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class InsertTrackCommand : public Command
{
public:
	InsertTrackCommand( TrackNr const trackNr )
		: m_trackNr( trackNr )
	{ }

	~InsertTrackCommand( ) { }

	virtual void Do( NNetModelWriterInterface & model ) 
	{
		model.GetMonitorData()->InsertTrack( m_trackNr );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{
		model.GetMonitorData()->DeleteTrack( m_trackNr );
	}

private:
	TrackNr m_trackNr;
};

