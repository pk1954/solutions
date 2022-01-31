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
	explicit BeeperThread(Sound * const pSound)
		: m_pSound(pSound)
	{ 
		StartThread(L"BeeperThread", true);
	}

	void Beep(Hertz hertz, MilliSecs msecs)
	{
		PostThreadMsg(IDM_BEEP, static_cast<WPARAM>(hertz.GetValue()), static_cast<LPARAM>(msecs.GetValue()));
	}

	void ThreadMsgDispatcher(MSG const msg) final
	{
		assert(msg.message == IDM_BEEP);
		SoundDescr desc 
		{ 
			true, 
			static_cast<Hertz>    (Cast2UnsignedLong(msg.wParam)), 
			static_cast<MilliSecs>(Cast2UnsignedLong(msg.lParam)) 
		};
//		m_pSound->Beep(desc);
		m_pSound->Play(TEXT("SNAP_IN_SOUND"));
	}

private:
	Sound * m_pSound;
};
