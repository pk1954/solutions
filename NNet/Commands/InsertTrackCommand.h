// InsertTrackCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MonitorData.h"
#include "NNetModelReaderInterface.h"
#include "Command.h"

class InsertTrackCommand : public Command
{
public:
	InsertTrackCommand
	( 
		TrackNr       const trackNr,
		MonitorData * const pMonitorData
	)
	{ 
		m_trackNr      = trackNr;
		m_pMonitorData = pMonitorData;
	}

	~InsertTrackCommand( ) { }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{
		m_pMonitorData->InsertTrack( m_trackNr );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		m_pMonitorData->DeleteTrack( m_trackNr );
	}

private:
	TrackNr       m_trackNr      { -1 };
	MonitorData * m_pMonitorData { nullptr }; 
};

