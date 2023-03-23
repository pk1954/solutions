// Preferences.ixx
//
// NNetWindows

module;

#include <compare>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>

export module NNetWin32:Preferences;

import SoundInterface;
import WrapBase;
import NNetModelIO;
import NNetModel;
import :DescriptionWindow;

using std::wstring;
using std::vector;
using std::unique_ptr;

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

	void SetArrows      (bool const);
	void SetScales      (bool const);
	void SetSensorPoints(bool const);

	bool ScalesVisible      () const { return m_bScales; }
	bool ArrowsVisible      () const { return m_bArrows; }
	bool SensorPointsVisible() const { return m_bSensorPoints; }

	enum class tInputCablesVisibility
	{
		all, nonStd, active, none
	};

	tInputCablesVisibility InputCablesVisibility() const { return m_inputCablesVisibility; }
	void SetInputCablesVisibility(tInputCablesVisibility v) { m_inputCablesVisibility = v; }

private:
	HWND                             m_hwndApp               { nullptr };
	tInputCablesVisibility           m_inputCablesVisibility { tInputCablesVisibility::nonStd };
	bool                             m_bScales               { false };
	bool                             m_bArrows               { false };
	bool                             m_bSensorPoints         { false };
	NNetModelReaderInterface const * m_pNMRI                 { nullptr };
	vector<unique_ptr<WrapBase>>     m_prefVector;
	wstring                          m_wstrPreferencesFile;
};
