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

	void Start
	( 
		EvolutionCore * const pCore,
		HistorySystem * const pHistSys
	)
	{
		m_pCore = pCore;
		m_pHistorySystem = pHistSys;
	}

	virtual void Notify( bool const );

private:
	EvolutionCore * m_pCore;
	HistorySystem * m_pHistorySystem;
};
