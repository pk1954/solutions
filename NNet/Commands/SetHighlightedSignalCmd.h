// SetHighlightedSignalCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Signal.h"
#include "Command.h"

class SetHighlightedSignalCmd : public Command
{
public:
	SetHighlightedSignalCmd(MicroMeterPnt const & umPos)
      : m_umPos(umPos)
	{ }

	virtual void Do()
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
