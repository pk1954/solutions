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
class MainWindow;
class Sound;

class Preferences
{
public:
	void Initialize
	(
		NNetModelReaderInterface &,
		DescriptionWindow &,
		MainWindow &,
		Sound &, 
		NNetModelImporter &, 
		HWND const
	);
	bool ReadPreferences () const;
	bool WritePreferences() const;

private:
	vector<unique_ptr<WrapBase>> m_prefVector;
};
