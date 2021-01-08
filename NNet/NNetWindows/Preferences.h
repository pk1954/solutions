// Preferences.h
//
// NNetWindows

#pragma once

#include <string>

using std::wstring;

class NNetModelImport;
class Sound;

class Preferences
{
public:
	void Initialize( Sound &, NNetModelImport & );
	bool ReadPreferences( );
	bool WritePreferences( wstring const );

private:
	Sound * m_pSound;
};
