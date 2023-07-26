// NNetPreferences.ixx
//
// NNetWindows

module;

#include "Resource.h"
#include <compare>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>

export module NNetWin32:NNetPreferences;

import Win32_Util_Resource;
import BoolType;
import SoundInterface;
import WrapBase;
import WinManager;
import NNetModelIO;
import NNetModel;
import Preferences;
import :DescriptionWindow;

using std::wostream;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::make_unique;

export class ShowArrowsBoolType : public BoolType
{
public:
	using BoolType::BoolType;

	bool Animate(bool const bShow)
	{
		bool const bOld = BoolType::Set(bShow);
		WinManager::SendCommand(RootWinId(IDM_MAIN_WINDOW), IDD_ARROW_ANIMATION, true);
		return bOld;
	}
};

export class NNetPreferences: public Preferences
{
public:
	void Initialize(NNetModelIO&);

	void SetModelInterface(NNetModelReaderInterface const *);

	enum class tInputCablesVisibility { all, nonStd, active, none };

	tInputCablesVisibility InputCablesVisibility() const { return m_inputCablesVisibility; }
	void SetInputCablesVisibility(tInputCablesVisibility v) { m_inputCablesVisibility = v; }

	NNetModelReaderInterface const *GetModelInterface() const { return m_pNMRI; };
	NNetModelIO                    &GetModelIO()              { return *m_pModelIO; }

	ShowArrowsBoolType m_bArrows       { false };
	BoolType           m_bSensorPoints { false };

private:

	template <Wrap_t WRAPPER>
	void Add(wstring const& name)
	{
		AddWrapper(make_unique<WRAPPER>(name, *this));
	}

	NNetModelIO                    * m_pModelIO              { nullptr };
	tInputCablesVisibility           m_inputCablesVisibility { tInputCablesVisibility::nonStd };
	NNetModelReaderInterface const * m_pNMRI                 { nullptr };
};
