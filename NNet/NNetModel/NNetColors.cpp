// NNetColors.cpp
//
// NNetModel

#include "stdafx.h"
#include "win32_thread.h"
#include "NNetColors.h"

VOID CALLBACK BlinkTimerCallback
( 
	PTP_CALLBACK_INSTANCE pInstance,
	PVOID pCOntext,
	PTP_TIMER pTimer
)
{
	NNetColors::m_colSelected = NNetColors::m_bSwitch 
		                        ? NNetColors::INT_BLINK_KEY_2 
		                        : NNetColors::INT_BLINK_KEY_1;
	NNetColors::m_bSwitch = ! NNetColors::m_bSwitch;
	if ( NNetColors::m_pObservable )
		NNetColors::m_pObservable->NotifyAll( false );
}

NNetColors::NNetColors( )
{
	m_pTimer = CreateThreadpoolTimer( BlinkTimerCallback, nullptr, nullptr );
	__declspec(align(8)) FILETIME ft { 0, 0 };
	SetThreadpoolTimer(	m_pTimer, &ft, BLINK_TIME.GetValue(), 100 );
}

void NNetColors::Initialize( Observable * const pObservable )
{
	m_pObservable = pObservable;
}

NNetColors::~NNetColors( )
{
	m_pObservable = nullptr;
	CloseThreadpoolTimer( m_pTimer );
}
