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
	void Initialize( Sound &, NNetModelImporter &, HWND const );
	bool ReadPreferences();
	bool WritePreferences( wstring const );

private:
	HWND    m_hwndApp;
	Sound * m_pSound;
};
