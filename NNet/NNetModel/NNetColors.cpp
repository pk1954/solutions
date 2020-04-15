// NNetColors.cpp
//
// NNetModel

#include "stdafx.h"
#include "win32_thread.h"
#include "NNetColors.h"

void NNetColors::SetColSelectedAndSleep( D2D1::ColorF const color )
{
	m_colSelected = color;
	Sleep( BLINK_TIME.GetValue() );
}

unsigned int __stdcall BlinkFunc( void * pData )
{
	NNetColors * pNNetColors { static_cast<NNetColors *>( pData ) };

	for (;;)
	{
		pNNetColors->SetColSelectedAndSleep( NNetColors::INT_BLINK_KEY_1 );
		pNNetColors->SetColSelectedAndSleep( NNetColors::INT_BLINK_KEY_2 );
	}
}

NNetColors::NNetColors( ObserverInterface * const pObserver )
	: m_pObserver( pObserver )
{
	Util::RunAsAsyncThread( BlinkFunc, this );
}
