// SlowMotionRatio.h
//
// Utilities

#pragma once

#include "observable.h"

class SlowMotionRatio : public Observable
{
public:

	static unsigned int const DEFAULT =   1000;
	static unsigned int const MIN     =    100;
	static unsigned int const MAX     = 100000;

	SlowMotionRatio( )
	  : m_ratio( DEFAULT )
	{
	}

	unsigned int GetRatio( )
	{
		return m_ratio;
	}

	void SetRatio( unsigned int const ratio )
	{
		m_ratio = ratio;
		NotifyAll( false );
	}

private:
	unsigned int m_ratio;   // 1 means realtime, 10 means slow motion factor 10
};