// Win32_Sound.ixx
//
// Toolbox\Win32_utilities

module;

#include <Windows.h>
#include "mmsystem.h"

export module Win32_Sound;

import std;
import Types;
import IoUtil;
import SoundInterface;

using std::wstring;
using std::vector;

export class WinSound : public Sound
{
public:

	void Play(vector<unsigned char> const &data) const final
	{
		if (Preferences::m_bSound.Get())
		::PlaySoundA((LPCSTR)data.data(), NULL, SND_MEMORY | SND_ASYNC | SND_NOSTOP);
	}

	void Beep(SoundDescr const & desc) const final
	{
		if (
			Preferences::m_bSound.Get() && desc.m_bOn &&
			(desc.m_frequency >= 37_Hertz) &&  // winapi limit 37 Hertz
			(desc.m_duration > 0_MilliSecs) 
		   ) 
		{
			::Beep
			(
				desc.m_frequency.GetValue(), 
				desc.m_duration .GetValue()
			);
		}
	}

	void Warning() const final
	{
		if (Preferences::m_bSound.Get())
			MessageBeep(MB_ICONWARNING);
	}
};
