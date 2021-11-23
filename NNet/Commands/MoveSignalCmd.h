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
	  : m_signalIdOld(id),
		m_trackNrNew(trackNr)
	{}

	void Do() final 
	{ 
		m_trackNrOld  = m_signalIdOld.GetTrackNr();
		m_signalNrNew = m_pNMWI->GetMonitorData().MoveSignal(m_signalIdOld, m_trackNrNew);
	}

	virtual void Undo() 
	{
		m_pNMWI->GetMonitorData().MoveSignal(SignalId(m_trackNrNew, m_signalNrNew), m_trackNrOld);
	}

private:
	SignalId const m_signalIdOld;
	TrackNr  const m_trackNrNew;
	TrackNr        m_trackNrOld;
	SignalNr       m_signalNrNew;
};
