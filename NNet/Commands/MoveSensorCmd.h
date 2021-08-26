// MoveSensorCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Signal.h"
#include "MonitorData.h"

class MoveSensorCmd : public Command
{
public:
	MoveSensorCmd
	(
		SignalId      const & id,
		MicroMeterPnt const & delta
	)
	  : m_signalId(id),
		m_delta(delta)
	{}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		Signal * pSignal { nmwi.GetMonitorData().GetSignalPtr(m_signalId) };
		assert(pSignal);
		pSignal->MoveSensor(m_delta);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		Signal * pSignal { nmwi.GetMonitorData().GetSignalPtr(m_signalId) };
		assert(pSignal);
		pSignal->MoveSensor(-m_delta);
	}

	virtual bool const CombineCommands(Command const & src) 
	{ 
		MoveSensorCmd const & cmdSrc { static_cast<MoveSensorCmd const &>(src) };
		if (m_signalId != cmdSrc.m_signalId)
			return false;
		m_delta += cmdSrc.m_delta;
		return true; 
	};

private:
	SignalId      m_signalId;
	MicroMeterPnt m_delta;
};
