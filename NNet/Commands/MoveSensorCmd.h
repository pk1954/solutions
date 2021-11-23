// MoveSensorCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Signal.h"
#include "MonitorData.h"

class MoveSensorCmd : public NNetCommand
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

	void Do() final 
	{ 
		Signal * pSignal { m_pNMWI->GetMonitorData().GetSignalPtr(m_signalId) };
		assert(pSignal);
		pSignal->MoveSensor(m_delta);
	}

	virtual void Undo() 
	{ 
		Signal * pSignal { m_pNMWI->GetMonitorData().GetSignalPtr(m_signalId) };
		assert(pSignal);
		pSignal->MoveSensor(-m_delta);
	}

	virtual bool CombineCommands(Command const & src) 
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
