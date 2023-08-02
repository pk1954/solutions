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
import WrapBase;
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
	static void Initialize(NNetModelIO&);

	static void SetModelInterface(NNetModelReaderInterface const *);

	enum class tInputCablesVisibility { all, nonStd, active, none };

	static tInputCablesVisibility InputCablesVisibility()   { return m_inputCablesVisibility; }
	static void SetInputCablesVisibility(tInputCablesVisibility v) { m_inputCablesVisibility = v; }

	static NNetModelReaderInterface const *GetModelInterface() { return m_pNMRI; };
	static NNetModelIO                    &GetModelIO()        { return *m_pModelIO; }

	inline static ShowArrows m_bArrows;
	inline static BoolType   m_bSensorPoints { false };

private:

	inline static NNetModelIO                    * m_pModelIO              { nullptr };
	inline static tInputCablesVisibility           m_inputCablesVisibility { tInputCablesVisibility::nonStd };
	inline static NNetModelReaderInterface const * m_pNMRI                 { nullptr };
};
