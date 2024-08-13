// SoundInterface.ixx
//
// Toolbox\Utilities

export module SoundInterface;

import std;
import Observable;
import BoolType;
import Types;

using std::wstring;
using std::vector;

export struct SoundDescr
{
	bool      m_bOn       { false };
	Hertz     m_frequency { 0_Hertz };      
	MilliSecs m_duration  { 0_MilliSecs };
};

export class Sound
{
public:
	virtual ~Sound() = default;

	virtual void Play(vector<unsigned char> const &) const = 0;
	virtual void Beep(SoundDescr            const &) const = 0;
	virtual void WarningSound()                      const = 0;
};
