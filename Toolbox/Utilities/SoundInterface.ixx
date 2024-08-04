// SoundInterface.ixx
//
// Toolbox\Utilities

export module SoundInterface;

import std;
import Observable;
import BoolType;
import Types;

using std::wstring;

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

	virtual void Play(wstring    const &) const = 0;
	virtual void Beep(SoundDescr const &) const = 0;
	virtual void Warning()                const = 0;
};
