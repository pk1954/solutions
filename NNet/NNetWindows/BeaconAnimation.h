// BeaconAnimation.h
//
// NNetWindows

#pragma once

#include <cmath>
#include "MoreTypes.h"
#include "ShapeId.h"
#include "win32_thread.h"

class BeaconAnimation : public Util::Thread
{
public:
	void Start( ShapeId const id )
	{
		reset();
		m_idBeacon  = id;
		m_increment =  1_PERCENT;
		StartThread( L"BeaconAnimation", true );
		PostThreadMsg( 0, 0, 0 );
	}

	void Stop( )
	{
		TerminateNoWait();
		m_idBeacon = NO_SHAPE;
	}

	PERCENT GetPercentage( ) const
	{
		return m_percentage;
	}

	ShapeId GetBeaconShapeId( ) const
	{
		return m_idBeacon;
	}

private:
	PERCENT m_percentage;
	PERCENT m_increment;
	ShapeId m_idBeacon { NO_SHAPE };

	void reset( )
	{
		m_percentage = 20_PERCENT;
	}

	void ThreadMsgDispatcher( MSG msg )
	{
		if ( m_percentage == 100_PERCENT )
			reset();
		else
			m_percentage += m_increment;
		Sleep( 1 );
		PostThreadMsg( msg.message, msg.wParam, msg.lParam ); // do it again
	}
};
