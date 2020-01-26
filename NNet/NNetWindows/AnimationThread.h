// AnimationThread.h
//
// NNetWindows

#pragma once

#include <cmath>
#include "MoreTypes.h"
#include "Pipeline.h"
#include "win32_thread.h"

class AnimationThread : public Util::Thread
{
public:
	AnimationThread()
		: m_umTarget( 0.0_MicroMeter )
	{
		StartThread( L"Animation", true );
	}

	void SetTarget( MicroMeter const umNewTarget )
	{
		m_umTarget = umNewTarget;
		m_umIncrement = (m_umTarget - Pipeline::GetArrowSize()) / 100.0f;
		PostThreadMsg( 0, 0, 0 );
	}

private:

	MicroMeter m_umTarget;
	MicroMeter m_umIncrement;

	void ThreadMsgDispatcher( MSG msg )
	{
		MicroMeter umActSize { Pipeline::GetArrowSize() };
		if ( fabs(umActSize.GetValue() - m_umTarget.GetValue()) < fabs(m_umIncrement.GetValue()) )
		{
			Pipeline::SetArrowSize( m_umTarget );
		}
		else
		{
			Pipeline::SetArrowSize( umActSize + m_umIncrement );
			Sleep( 10 );
			PostThreadMsg( msg.message, msg.wParam, msg.lParam ); // do it again
		}
	}
};
