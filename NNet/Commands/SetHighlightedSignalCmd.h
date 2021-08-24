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

	virtual void Do(NNetModelWriterInterface & nmwi)
	{
		m_pSignalIdOld = nmwi.GetMonitorData().SetHighlightedSignal(m_umPos);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi)
	{
		nmwi.GetMonitorData().SetHighlightedSignal(m_pSignalIdOld);
	}

private:
	MicroMeterPnt m_umPos; 
	SignalId      m_pSignalIdOld {};
};
