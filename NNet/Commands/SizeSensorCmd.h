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
      : m_pSignal(m_pNMWI->GetMonitorData().GetSignalPtr(id)),
		m_signalId(id),
		m_fFactor(fFactor)
	{}

	void Do  () final 
	{ 
		if (m_pSignal)
			m_pSignal->SizeSensor(1.0f / m_fFactor); 
	}

	void Undo() final 
	{ 
		if (m_pSignal)
			m_pSignal->SizeSensor(m_fFactor); 
	}

	bool CombineCommands(Command const & src) final
	{ 
		SizeSensorCmd const & cmdSrc { static_cast<SizeSensorCmd const &>(src) };
		if (m_signalId != cmdSrc.m_signalId)
			return false;
		m_fFactor *= cmdSrc.m_fFactor;
		return true; 
	};

private:
	Signal * m_pSignal;
	SignalId m_signalId;
	float    m_fFactor;
};
