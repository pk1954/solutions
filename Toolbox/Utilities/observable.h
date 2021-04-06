// observable.h : 
//
// Utilities

#pragma once

#include "observerInterface.h"
#include "ViewCollection.h"

class Observable
{
public:
	virtual ~Observable() {};

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

	void UnregisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Unregister( pObserver );
	}

	void UnregisterAllObservers()
	{
		m_observers.Clear();
	}

	void NotifyAll( bool const bImmediately )
	{
		m_observers.NotifyAll( bImmediately );
	}

private:
	ViewCollection m_observers;
};
