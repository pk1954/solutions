// AnimationThread.h
//
// NNetWindows

#pragma once

#include <cmath>
#include "MoreTypes.h"
#include "Pipe.h"
#include "win32_thread.h"

class AnimationThread : public Util::Thread
{
public:
	AnimationThread()
	{
		StartThread( L"Animation", true );
	}

	void SetTarget( MicroMeter const umNewTarget )
	{
		m_umTarget = umNewTarget;
		m_umIncrement = (m_umTarget - Pipe::GetArrowSize()) / 100.0f;
		PostThreadMsg( 0, 0, 0 );
	}

private:

	MicroMeter m_umTarget    { 0.0_MicroMeter };
	MicroMeter m_umIncrement { 0.0_MicroMeter };

	void ThreadMsgDispatcher( MSG msg )
	{
		MicroMeter umActSize { Pipe::GetArrowSize() };
		if ( fabs(umActSize.GetValue() - m_umTarget.GetValue()) < fabs(m_umIncrement.GetValue()) )
		{
			Pipe::SetArrowSize( m_umTarget );
		}
		else
		{
			Pipe::SetArrowSize( umActSize + m_umIncrement );
			Sleep( 10 );
			PostThreadMsg( msg.message, msg.wParam, msg.lParam ); // do it again
		}
	}
};
