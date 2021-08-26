// SizeSensorCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Signal.h"
#include "MonitorData.h"

class SizeSensorCmd : public Command
{
public:
	SizeSensorCmd
	(
		SignalId const & id,
		float    const   fFactor
	)
      : m_signalId(id),
		m_fFactor(fFactor)
	{}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		Signal * pSignal = nmwi.GetMonitorData().GetSignalPtr(m_signalId);
		assert(pSignal);
		pSignal->SizeSensor(1.0f / m_fFactor);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		Signal * pSignal = nmwi.GetMonitorData().GetSignalPtr(m_signalId);
		assert(pSignal);
		pSignal->SizeSensor(m_fFactor);
	}

	virtual bool const CombineCommands(Command const & src) 
	{ 
		SizeSensorCmd const & cmdSrc { static_cast<SizeSensorCmd const &>(src) };
		if (m_signalId != cmdSrc.m_signalId)
			return false;
		m_fFactor *= cmdSrc.m_fFactor;
		return true; 
	};

private:
	SignalId m_signalId;
	float    m_fFactor;
};
