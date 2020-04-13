// Preferences.h
//
// NNetWindows

#pragma once

#include <string>

using std::wstring;

class NNetModelStorage;
class NNetWindow;
class NNetModel;
class Param;

class Preferences
{
public:
	static void Initialize( );

	static bool ReadPreferences( NNetModelStorage *, NNetModel *, Param * );
	static bool WritePreferences( wstring const, NNetWindow const * const );

private:
};
