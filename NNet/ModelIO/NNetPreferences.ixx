// NNetPreferences.ixx
//
// ModelIO

module;

#include "Resource.h"
#include <compare>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>

export module NNetPreferences;

import Win32_Util_Resource;
import BoolType;
import SoundInterface;
import Wrapper;
import WinManager;
import NNetModelIO;
import NNetModel;
import Preferences;

using std::wostream;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::make_unique;

class ShowArrows : public BoolType
{
public:
	ShowArrows()
	  : BoolType(false)
	{}

	void Toggle() final // comes from menu
	{
		BoolType::Toggle();
		WinManager::SendCommand(RootWinId(IDM_MAIN_WINDOW), IDD_ARROW_ANIMATION, true);
	}

	bool Set(bool const bActive) final  // comes from preferences script
	{
		bool const bOld = BoolType::Set(bActive);
		WinManager::SendCommand(RootWinId(IDM_MAIN_WINDOW), IDD_ARROW_ANIMATION, false);
		return bOld;
	}
};

export class NNetPreferences
{
public:
	static void Initialize();

	static void SetModelInterface(NNetModelReaderInterface const *);

	enum class tInputCablesVisibility { all, nonStd, active, none };

	static tInputCablesVisibility InputCablesVisibility()   { return m_inputCablesVisibility; }
	static void SetInputCablesVisibility(tInputCablesVisibility v) { m_inputCablesVisibility = v; }

	static NNetModelReaderInterface const *GetModelInterface() { return m_pNMRI; };

	static bool ScanAreaVisible() { return m_bScanArea.Get(); }
	static bool ModelFront     () { return m_bModelFront.Get(); }
	static bool ApplyFilter    () { return m_bFilter.Get(); }

	inline static ShowArrows m_bArrows;
	inline static BoolType   m_bSensorPoints { false };
	inline static BoolType   m_bScanArea     { false };
	inline static BoolType   m_bModelFront   { true  };
	inline static BoolType   m_bFilter       { false };

private:

	inline static tInputCablesVisibility           m_inputCablesVisibility { tInputCablesVisibility::nonStd };
	inline static NNetModelReaderInterface const * m_pNMRI                 { nullptr };
};
