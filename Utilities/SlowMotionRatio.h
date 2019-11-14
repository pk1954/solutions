// SlowMotionRatio.h
//
// Utilities

#pragma once

#include <array>
#include "observable.h"

class SlowMotionRatio : public Observable
{
public:

	SlowMotionRatio( )
	  : m_ratioIndex( DEFAULT_INDEX )
	{
	}

	unsigned int GetRatio( )
	{
		return m_ratios[ m_ratioIndex ];
	}

	bool IncRatio( )
	{
		if ( m_ratioIndex < MAX_INDEX )
		{
			setIndex( m_ratioIndex + 1 );
			return true;
		}
		else 
			return false;
	}

	bool DecRatio( )
	{
		if ( m_ratioIndex > 0 )
		{
			setIndex( m_ratioIndex - 1 );
			return true;
		}
		else 
			return false;
}

private:
	static unsigned int const DEFAULT_INDEX =  6;
	static unsigned int const MAX_INDEX     = 18;

	static std::array< unsigned int, MAX_INDEX + 1 > const m_ratios;

	unsigned int m_ratioIndex;   // index to m_ratios

	void setIndex( unsigned int const index )
	{
		m_ratioIndex = index;
		NotifyAll( true );
	}
};