// win32_sound.h
//
// Toolbox\Win32_utilities

#pragma once

#include <string>
#include "Windows.h"
#include "MoreTypes.h"
#include "Mmsystem.h"
#include "SoundInterface.h"

using std::wstring;

class WinSound : public Sound
{
public:

	virtual void Play( wstring const sound ) const 
	{
		if ( IsOn() )
		{
			::PlaySound( sound.c_str(), GetModuleHandle(NULL), SND_RESOURCE|SND_ASYNC|SND_NOSTOP ); 
		}
	}

	virtual void Beep( SoundDescr const & desc ) const
	{
		if ( 
			  IsOn() && desc.m_bOn && 
			  (desc.m_frequency >= 37_Hertz) &&  // winapi limit 37 Hertz
			  (desc.m_duration > 0_MilliSecs) 
		   ) 
		{
//			Play( TEXT("SNAP_IN_SOUND") );
		   ::Beep( static_cast<DWORD>(desc.m_frequency.GetValue()), static_cast<DWORD>(desc.m_duration.GetValue()) );
		}
	}

private:
};
