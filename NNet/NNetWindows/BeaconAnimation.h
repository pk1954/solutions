// BeaconAnimation.h
//
// NNetWindows

#pragma once

#include "Observable.h"                   
#include "MoreTypes.h"
#include "ShapeId.h"
#include "AnimationInterface.h"
#include "win32_thread.h"

class BeaconAnimation : public Util::Thread, public AnimationInterface
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

	virtual void Start( MicroMeterCircle const * const pCircle )
	{
		reset();
		m_pCircle = pCircle;
		PostThreadMsg( 0, 0, 0 );
	}

	virtual void Stop( )
	{
		m_pCircle = nullptr;
	}

	PERCENT GetPercentage( ) const
	{
		return m_percentage;
	}

	MicroMeterCircle const * const GetSensorCircle( ) const
	{
		return m_pCircle;
	}

private:
	PERCENT                  m_percentage  { };
	PERCENT                  m_increment   { 1_PERCENT };
	MicroMeterCircle const * m_pCircle     { nullptr };
	Observable             * m_pObservable { nullptr };

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
		if ( m_pCircle )
			PostThreadMsg( msg.message, msg.wParam, msg.lParam ); // do it again
	}
};
