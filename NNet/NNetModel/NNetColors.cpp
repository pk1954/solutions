// NNetColors.cpp
//
// NNetModel

#include "stdafx.h"
#include "win32_thread.h"
#include "NNetColors.h"

D2D1::ColorF const NNetColors::EXT_NORMAL         { 0.0f, 0.5f, 1.0f, 1.0f };
D2D1::ColorF const NNetColors::EXT_HIGHLIGHT      { 0.0f, 0.8f, 0.8f, 1.0f };
D2D1::ColorF const NNetColors::EXT_SUPER_HIGHLIGHT{ 1.0f, 0.0f, 0.0f, 1.0f };
D2D1::ColorF const NNetColors::EXT_LOW_KEY        { 0.5f, 0.5f, 0.5f, 1.0f };
D2D1::ColorF const NNetColors::INT_LOW_KEY        { 0.9f, 0.9f, 0.9f, 1.0f };
D2D1::ColorF const NNetColors::SELECTION_RECT     { 1.0f, 0.0f, 0.0f, 0.5f };

unsigned int __stdcall BlinkFunc( void * pData )
{
	D2D1::ColorF const INT_SELECTED { 1.0f, 0.6f, 0.1f, 1.0f };

	NNetColors * pNNetColors { static_cast<NNetColors *>( pData ) };

	for (;;)
	{
		pNNetColors->SetColSelectedAndSleep( INT_SELECTED );
		pNNetColors->SetColSelectedAndSleep( NNetColors::INT_LOW_KEY );
	}
}

D2D1::ColorF NNetColors::m_colSelected { NNetColors::INT_LOW_KEY };

NNetColors::NNetColors( ObserverInterface * const pObserver )
	: m_pObserver( pObserver )
{
	Util::RunAsAsyncThread( BlinkFunc, this );
}
