// Preferences.h
//
// NNetWindows

#pragma once

#include <string>
#include <vector>
#include "WrapBase.h"

using std::wstring;
using std::vector;

class NNetModelIO;
class NNetModelReaderInterface;
class DescriptionWindow;
class MainWindow;
class Sound;

class Preferences
{
public:
	void Initialize
	(
		DescriptionWindow &,
		MainWindow &,
		Sound &, 
		NNetModelIO &, 
		HWND const
	);
	void SetModelInterface(NNetModelReaderInterface const *);
	NNetModelReaderInterface const * GetModelInterface() const { return m_pNMRI; };
	bool ReadPreferences () const;
	bool WritePreferences() const;

private:
	NNetModelReaderInterface const * m_pNMRI { nullptr };
	vector<unique_ptr<WrapBase>>     m_prefVector;
	wstring                          m_wstrPreferencesFile;
};
