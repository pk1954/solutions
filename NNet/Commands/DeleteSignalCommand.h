// DeleteSignalCommand.h
//
// Commands

#pragma once

#include "Signal.h"
#include "MonitorData.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class DeleteSignalCommand : public Command
{
public:
	DeleteSignalCommand( SignalId const & id )
	  : m_signalId( id ),
		m_pSignal( nullptr )
	{ }

	~DeleteSignalCommand( ) { }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{
		m_pSignal = pModel->GetMonitorData()->DeleteSignal( m_signalId );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		m_signalId.signalNr = pModel->GetMonitorData()->AddSignal( m_signalId.trackNr, m_pSignal );
	}

private:
	SignalId m_signalId;
	Signal * m_pSignal;
};

