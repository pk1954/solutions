// win32_sound.h
//
// NNetWindows

#pragma once

#include "Windows.h"
#include "Mmsystem.h"

inline void PlaySound( LPCWSTR const pszSound )
{
	::PlaySound( pszSound, GetModuleHandle(NULL) , SND_RESOURCE|SND_ASYNC ); 
}