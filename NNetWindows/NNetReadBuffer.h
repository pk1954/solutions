// NNetReadBuffer.h 
//
// NNetWindows

#pragma once

#include "observable.h"
#include "observerInterface.h"

class NNetReadBuffer : public ObserverInterface,  // Observes producer thread
	                   public Observable          // Can be observed by consumer threads
{
public:
	NNetReadBuffer( ) {	}

	~NNetReadBuffer( ) { }

	// called by producer thread

	virtual void Notify( bool const bRunMode )  // In run mode, performance of producer thread has top priority
	{                                           // immediate refresh of UI is secondary 
		bool bImmediate = ! bRunMode;         
		NotifyAll( bImmediate );                // Notify Observers  
	}
};
