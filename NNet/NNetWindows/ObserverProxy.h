// ObserverProxy.h 
//
// NNetWindows

#pragma once

#include "observable.h"
#include "observerInterface.h"

class ObserverProxy : public ObserverInterface,   // Observes producer thread
	                  public Observable           // Can be observed by consumer threads
{
public:
	ObserverProxy( ) { }

	~ObserverProxy( ) { }

	virtual void Notify( bool const bImmediate )  
	{                                          
		NotifyAll( bImmediate );     
	}
};
