// win32_sound.h
//
// Toolbox\Win32_utilities

#pragma once

#include "Windows.h"
#include "MoreTypes.h"
#include "Mmsystem.h"

struct SoundDescr
{
	bool      m_bOn       { false };
	Hertz     m_frequency { 0_Hertz };      
	MilliSecs m_duration  { 0_MilliSecs };
};

class Sound
{
public:

	static void Play( LPCWSTR const pszSound )
	{
		if ( m_bActive )
			::PlaySound( pszSound, GetModuleHandle(NULL), SND_RESOURCE|SND_ASYNC|SND_NOSTOP ); 
	}

	static void Beep( SoundDescr const & desc )
	{
		if ( 
			  m_bActive && desc.m_bOn && 
			  (desc.m_frequency >= 37_Hertz) &&  // winapi limit 37 Hertz
			  (desc.m_duration > 0_MilliSecs) 
		   ) 
		   ::Beep( static_cast<DWORD>(desc.m_frequency.GetValue()), static_cast<DWORD>(desc.m_duration.GetValue()) );
	}

	//static void Beep( Hertz hertz, MilliSecs msecs )
	//{
	//	if ( m_bActive && (hertz >= 37_Hertz) && (msecs > 0_MilliSecs) )  // winapi limit 37 Hertz
	//		::Beep( static_cast<DWORD>(hertz.GetValue()), static_cast<DWORD>(msecs.GetValue()) );
	//}

	static void On ( ) { m_bActive = true; }
	static void Off( ) { m_bActive = false; }

	static bool IsOn( ) { return m_bActive; }

private:

	inline static bool m_bActive { true };
};
