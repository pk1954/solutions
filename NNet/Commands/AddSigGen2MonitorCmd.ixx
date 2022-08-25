// AddSigGen2MonitorCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "SignalFactory.h"
#include "MonitorData.h"
#include "NNetCommand.h"
#include "SignalId.h"

export module AddSigGen2MonitorCmd;

import TrackNr;

export class AddSigGen2MonitorCmd : public NNetCommand
{
public:
	explicit AddSigGen2MonitorCmd(TrackNr const trackNr)
		: m_trackNr(trackNr)
	{
		m_pSigGen = m_pNMWI->GetSigGenSelected();
	}

	void Do() final
	{
		m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
		m_signalId = SignalFactory::MakeSigGenSignal(m_trackNr, *m_pNMWI);
	}

	void Undo() final
	{
		m_pNMWI->GetMonitorData().DeleteSignal(m_signalId);
		m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
	};

private:
	SignalGenerator const* m_pSigGen;
	TrackNr          const m_trackNr;
	SignalId               m_signalId{};
};