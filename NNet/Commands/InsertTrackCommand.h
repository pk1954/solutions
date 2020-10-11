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

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{
		pModel->GetMonitorData()->InsertTrack( m_trackNr );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		pModel->GetMonitorData()->DeleteTrack( m_trackNr );
	}

private:
	TrackNr m_trackNr;
};

