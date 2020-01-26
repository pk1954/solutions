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

	virtual void Notify( bool const bImmediate )  
	{                                          
		NotifyAll( bImmediate );     
	}
};
