// MoveSignalCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Signal.h"
#include "SignalId.h"
#include "Track.h"
#include "MonitorData.h"

export module MoveSignalCmd;

import TrackNr;

export class MoveSignalCmd : public NNetCommand
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

	void Undo() final 
	{
		m_pNMWI->GetMonitorData().MoveSignal(SignalId(m_trackNrNew, m_signalNrNew), m_trackNrOld);
	}

private:
	SignalId const m_signalIdOld;
	TrackNr  const m_trackNrNew;
	TrackNr        m_trackNrOld;
	SignalNr       m_signalNrNew;
};
