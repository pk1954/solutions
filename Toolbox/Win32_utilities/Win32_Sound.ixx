// Win32_Sound.ixx
//
// Toolbox\Win32_utilities

module;

#include <string>
#include "Windows.h"
#include "MoreTypes.h"
#include "mmsystem.h"

export module Win32_Sound;

import SoundInterface;

using std::wstring;

export class WinSound : public Sound
{
public:

	void Play(wstring const & sound) const final
	{
		if (IsOn())
		{
			::PlaySound(sound.c_str(), GetModuleHandle(NULL), SND_RESOURCE|SND_ASYNC|SND_NOSTOP); 
		}
	}

	void Beep(SoundDescr const & desc) const final
	{
		if (
			IsOn() && desc.m_bOn && 
			(desc.m_frequency >= 37_Hertz) &&  // winapi limit 37 Hertz
			(desc.m_duration > 0_MilliSecs) 
			) 
		{
			::Beep(static_cast<DWORD>(desc.m_frequency.GetValue()), static_cast<DWORD>(desc.m_duration.GetValue()));
		}
	}
};
