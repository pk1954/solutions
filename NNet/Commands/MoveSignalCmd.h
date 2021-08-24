// MoveSignalCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Signal.h"
#include "MonitorData.h"

class MoveSignalCmd : public Command
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

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_signalNr = nmwi.GetMonitorData().MoveSignal(m_signalId, m_trackNr);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{
		nmwi.GetMonitorData().MoveSignal(SignalId(m_trackNr, m_signalNr), m_signalId);
	}

private:
	SignalId m_signalId;
	TrackNr  m_trackNr;
	SignalNr m_signalNr;
};
