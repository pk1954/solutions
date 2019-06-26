// Win32_protocolServer.h 
//
// Win32_utilities

#pragma once

#include "observerInterface.h"

class EvolutionCore;
class HistorySystem;

class ProtocolServer : public ObserverInterface
{
public:
	ProtocolServer( );
	~ProtocolServer( );

	void Start( HistorySystem * const pHistSys )
	{
		m_pHistorySystem = pHistSys;
	}

	virtual void Notify( bool const );

private:
	HistorySystem * m_pHistorySystem;
};
