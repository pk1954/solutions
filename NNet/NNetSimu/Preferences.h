// Preferences.h
//
// NNetWindows

#pragma once

#include <string>

using std::wstring;

class NNetModelStorage;
class NNetWindow;
class NNetModel;

class Preferences
{
public:
	static void Initialize( );

	static bool ReadPreferences( NNetModelStorage *, NNetModel * );
	static bool WritePreferences( wstring const, NNetWindow const * const );

private:
};
