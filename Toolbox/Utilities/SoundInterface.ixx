// SoundInterface.ixx
//
// Toolbox\Utilities

module;

#include <string>

export module SoundInterface;

import Observable;
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

	void Initialize(Observable * const pObservable) const
	{
		m_pObservable = pObservable;
	}

	virtual void Play(wstring const &) const = 0;

	virtual void Beep(SoundDescr const &) const = 0;

	void On() const
	{ 
		m_bActive = true;
		m_pObservable->NotifyAll(true);
	}

	void Off() const
	{ 
		m_bActive = false; 
		m_pObservable->NotifyAll(true);
	}

	bool IsOn() const { return m_bActive; }

private:

	inline static Observable * m_pObservable { nullptr };
	inline static bool         m_bActive     { true };
};
