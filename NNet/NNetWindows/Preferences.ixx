// Preferences.ixx
//
// NNetWindows

module;

#include <string>
#include <vector>
#include "WrapBase.h"
#include "NNetModelIO.h"
#include "NNetModelReaderInterface.h"

export module Preferences;

import DescriptionWindow;
import MainWindow;

using std::wstring;
using std::vector;

import SoundInterface;

export class Preferences
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
