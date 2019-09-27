// SlowMotionRatio.h
//
// Utilities

#pragma once

#include "ViewCollection.h"

class SlowMotionRatio
{
public:

	static unsigned int const DEFAULT =  100;
	static unsigned int const MIN     =    1;
	static unsigned int const MAX     = 1000;

	unsigned int GetRatio( )
	{
		return m_ratio;
	}

	void SetRatio( unsigned int const ratio )
	{
		m_ratio = ratio;
		m_observers.NotifyAll( false );
	}

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

	void Stop( )
	{
		m_observers.Clear();
	}

private:
	unsigned int   m_ratio;   // 1 means realtime, 10 means slow motion factor 10
	ViewCollection m_observers;
};