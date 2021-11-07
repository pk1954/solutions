// Preferences.h
//
// NNetWindows

#pragma once

#include <string>
#include <vector>
#include "WrapBase.h"

using std::wstring;
using std::vector;

class NNetModelReaderInterface;
class NNetModelImporter;
class DescriptionWindow;
class Sound;

class Preferences
{
public:
	void Initialize
	(
		NNetModelReaderInterface &,
		DescriptionWindow &, 
		Sound &, 
		NNetModelImporter &, 
		HWND const
	);
	bool ReadPreferences();
	bool WritePreferences();

private:
	vector<unique_ptr<WrapBase>> m_prefVector;
};
