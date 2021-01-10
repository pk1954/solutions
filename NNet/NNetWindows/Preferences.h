// Preferences.h
//
// NNetWindows

#pragma once

#include <string>

using std::wstring;

class NNetModelImporter;
class Sound;

class Preferences
{
public:
	void Initialize( Sound &, NNetModelImporter & );
	bool ReadPreferences( );
	bool WritePreferences( wstring const );

private:
	Sound * m_pSound;
};
