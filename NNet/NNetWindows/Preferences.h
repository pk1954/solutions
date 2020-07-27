// Preferences.h
//
// NNetWindows

#pragma once

#include <string>

using std::wstring;

class NNetModelStorage;
class Sound;

class Preferences
{
public:
	void Initialize( Sound * );
	bool ReadPreferences( NNetModelStorage * );
	bool WritePreferences( wstring const );

private:
	Sound * m_pSound;
};
