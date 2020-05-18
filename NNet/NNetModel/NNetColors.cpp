// NNetColors.cpp
//
// NNetModel

#include "stdafx.h"
#include "win32_thread.h"
#include "NNetColors.h"

VOID CALLBACK TimerProc( HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime )
{
	NNetColors::m_colSelected = NNetColors::m_bSwitch 
		                        ? NNetColors::INT_BLINK_KEY_2 
		                        : NNetColors::INT_BLINK_KEY_1;
	NNetColors::m_bSwitch = ! NNetColors::m_bSwitch;
	if ( NNetColors::m_pObservable )
		NNetColors::m_pObservable->NotifyAll( false );
}

NNetColors::NNetColors( Observable * const pObservable )
{
	m_pObservable = pObservable;
	m_TimerId = SetTimer( NULL, 1, BLINK_TIME.GetValue(), TimerProc );
}

NNetColors::~NNetColors( )
{
	m_pObservable = nullptr;
	KillTimer( NULL, m_TimerId );
}
