// SizeSensorCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Signal.h"
#include "MonitorData.h"

class SizeSensorCmd : public NNetCommand
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

	void Do() final 
	{ 
		Signal * pSignal = m_pNMWI->GetMonitorData().GetSignalPtr(m_signalId);
		assert(pSignal);
		pSignal->SizeSensor(1.0f / m_fFactor);
	}

	void Undo() final 
	{ 
		Signal * pSignal = m_pNMWI->GetMonitorData().GetSignalPtr(m_signalId);
		assert(pSignal);
		pSignal->SizeSensor(m_fFactor);
	}

	virtual bool CombineCommands(Command const & src) 
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
