// win32_beeperThread.h 
//
// win32_utilities

#pragma once

#include "SoundInterface.h"
#include "win32_thread.h"
#include "win32_util_resource.h"

class BeeperThread: public Util::Thread
{
public:
	BeeperThread( Sound * const pSound )
		: m_pSound( pSound )
	{ 
		StartThread( L"BeeperThread", true );
	}

	~BeeperThread()	{ }

	void Beep( Hertz hertz, MilliSecs msecs )
	{
		PostThreadMsg( IDM_BEEP, static_cast<WPARAM>(hertz.GetValue()), static_cast<LPARAM>(msecs.GetValue()) );
	}

	virtual void ThreadMsgDispatcher( MSG const msg )
	{
		assert( msg.message == IDM_BEEP );
		SoundDescr desc 
		{ 
			true, 
			static_cast<Hertz>    (CastToUnsignedLong(msg.wParam)), 
			static_cast<MilliSecs>(CastToUnsignedLong(msg.lParam)) 
		};
		m_pSound->Beep( desc );
	}

private:
	Sound * m_pSound;
};
