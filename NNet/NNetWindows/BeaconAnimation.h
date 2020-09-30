// BeaconAnimation.h
//
// NNetWindows

#pragma once

#include "Observable.h"                   
#include "MoreTypes.h"
#include "ShapeId.h"
#include "win32_thread.h"

class BeaconAnimation : public Util::Thread
{
public:
	void Initialize( Observable * const pObservable )
	{
		m_pObservable = pObservable;
		StartThread( L"BeaconAnimation", true );
	}

	~BeaconAnimation( )
	{
		m_pObservable = nullptr;
		Stop();
		TerminateNoWait();
	}

	void Start( ShapeId const id )
	{
		reset();
		m_idBeacon = id;
		PostThreadMsg( 0, 0, 0 );
	}

	void Stop( )
	{
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
	PERCENT      m_percentage  { };
	PERCENT      m_increment   { 1_PERCENT };
	ShapeId      m_idBeacon    { NO_SHAPE };
	Observable * m_pObservable { nullptr };

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
		if ( m_pObservable )
			m_pObservable->NotifyAll( false );
		if ( IsDefined ( m_idBeacon ) )
			PostThreadMsg( msg.message, msg.wParam, msg.lParam ); // do it again
	}
};
