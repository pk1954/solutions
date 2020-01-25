// win32_sound.h
//
// NNetWindows

#pragma once

#include "Windows.h"
#include "Mmsystem.h"

class Sound
{
public:

static void Play( LPCWSTR const pszSound )
{
	if ( m_bActive )
		::PlaySound( pszSound, GetModuleHandle(NULL), SND_RESOURCE|SND_ASYNC ); 
}

static void On ( ) { m_bActive = true; }
static void Off( ) { m_bActive = false; }

static bool IsOn( ) { return m_bActive; }

private:
	static bool m_bActive;
};
