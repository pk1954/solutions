// MoveSignalCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Signal.h"
#include "MonitorData.h"

class MoveSignalCmd : public NNetCommand
{
public:
	MoveSignalCmd
	(
		SignalId const & id,
		TrackNr  const   trackNr
	)
	  : m_signalId(id),
		m_trackNr(trackNr)
	{}

	virtual void Do() 
	{ 
		m_signalNr = m_pNMWI->GetMonitorData().MoveSignal(m_signalId, m_trackNr);
	}

	virtual void Undo() 
	{
		m_pNMWI->GetMonitorData().MoveSignal(SignalId(m_trackNr, m_signalNr), m_signalId);
	}

private:
	SignalId m_signalId;
	TrackNr  m_trackNr;
	SignalNr m_signalNr;
};
