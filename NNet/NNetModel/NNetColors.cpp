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
	if ( NNetColors::m_pObserver )
		NNetColors::m_pObserver->Notify( false );
}

NNetColors::NNetColors( ObserverInterface * const pObserver )
{
	m_pObserver = pObserver;
	m_TimerId = SetTimer( NULL, 1, BLINK_TIME.GetValue(), TimerProc );
}

NNetColors::~NNetColors( )
{
	m_pObserver = nullptr;
	KillTimer( NULL, m_TimerId );
}
