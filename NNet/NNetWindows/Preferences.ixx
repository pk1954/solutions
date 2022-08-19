// Preferences.ixx
//
// NNetWindows

module;

#include <string>
#include <vector>
#include "NNetModelIO.h"
#include "NNetModelReaderInterface.h"

export module Preferences;

import DescriptionWindow;
import SoundInterface;
import WrapBase;

using std::wstring;
using std::vector;

export class Preferences
{
public:
	void Initialize
	(
		DescriptionWindow &,
		Sound &, 
		NNetModelIO &, 
		HWND const
	);
	void SetModelInterface(NNetModelReaderInterface const *);
	NNetModelReaderInterface const * GetModelInterface() const { return m_pNMRI; };
	bool ReadPreferences () const;
	bool WritePreferences() const;

	void SetArrows(bool const);
	void SetSensorPoints(bool const bOn) { m_bSensorPoints = bOn; }

	bool ArrowsVisible      () const { return m_bArrows; }
	bool SensorPointsVisible() const { return m_bSensorPoints; }

private:
	HWND                             m_hwndApp       { nullptr };
	bool                             m_bArrows       { false };
	bool                             m_bSensorPoints { false };
	NNetModelReaderInterface const * m_pNMRI         { nullptr };
	vector<unique_ptr<WrapBase>>     m_prefVector;
	wstring                          m_wstrPreferencesFile;
};
