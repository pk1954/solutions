// SoundInterface.h
//
// Toolbox\Utilities

#pragma once

#include <string>
#include "MoreTypes.h"

using std::wstring;

struct SoundDescr
{
	bool      m_bOn       { false };
	Hertz     m_frequency { 0_Hertz };      
	MilliSecs m_duration  { 0_MilliSecs };
};

class Sound
{
public:

	virtual void Play( wstring const ) const = 0;

	virtual void Beep( SoundDescr const & ) const = 0;

	void On ( ) { m_bActive = true; }
	void Off( ) { m_bActive = false; }

	bool IsOn( ) const { return m_bActive; }

private:

	inline static bool m_bActive { true };
};
