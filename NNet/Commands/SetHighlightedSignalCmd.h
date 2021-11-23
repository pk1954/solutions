// SetHighlightedSignalCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Signal.h"
#include "NNetCommand.h"

class SetHighlightedSignalCmd : public NNetCommand
{
public:
	SetHighlightedSignalCmd(MicroMeterPnt const & umPos)
      : m_umPos(umPos)
	{ }

	void Do() final
	{
		m_pSignalIdOld = m_pNMWI->GetMonitorData().SetHighlightedSignal(m_umPos);
	}

	virtual void Undo()
	{
		m_pNMWI->GetMonitorData().SetHighlightedSignal(m_pSignalIdOld);
	}

private:
	MicroMeterPnt m_umPos; 
	SignalId      m_pSignalIdOld {};
};
