// DeleteSignalCommand.h
//
// Commands

#pragma once

#include "Signal.h"
#include "MonitorData.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class DeleteSignalCommand : public Command
{
public:
	DeleteSignalCommand( TrackNr const trackNr, SignalNr const signalNr )
	  : m_trackNr( trackNr ),
		m_signalNr( signalNr ),
		m_pSignal( nullptr )
	{ }

	~DeleteSignalCommand( ) { }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{
		m_pSignal = pModel->GetMonitorData()->RemoveSignal( m_trackNr, m_signalNr );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		m_signalNr = pModel->GetMonitorData()->AddSignal( m_trackNr, m_pSignal );
	}

private:
	TrackNr  m_trackNr;
	SignalNr m_signalNr;
	Signal * m_pSignal;
};

