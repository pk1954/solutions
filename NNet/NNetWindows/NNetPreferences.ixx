// NNetPreferences.ixx
//
// NNetWindows

module;

#include <compare>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>

export module NNetWin32:NNetPreferences;

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

export class NNetPreferences: public Preferences
{
public:
	void Initialize
	(
		NNetModelIO&,
		WinManager&
	);

	void SetModelInterface(NNetModelReaderInterface const *);

	void SetArrows(bool const, bool const);

	enum class tInputCablesVisibility { all, nonStd, active, none };

	tInputCablesVisibility InputCablesVisibility() const { return m_inputCablesVisibility; }
	void SetInputCablesVisibility(tInputCablesVisibility v) { m_inputCablesVisibility = v; }

	NNetModelReaderInterface const *GetModelInterface() const { return m_pNMRI; };
	NNetModelIO                    &GetModelIO()              { return *m_pModelIO; }

	WinManager& GetWinManager() { return *m_pWinManager; }

	BoolType m_bArrows       { false };
	BoolType m_bSensorPoints { false };

private:

	template <Wrap_t WRAPPER>
	void Add(wstring const& name)
	{
		AddWrapper(make_unique<WRAPPER>(name, *this, *m_pWinManager));
	}

	NNetModelIO                    * m_pModelIO              { nullptr };
	tInputCablesVisibility           m_inputCablesVisibility { tInputCablesVisibility::nonStd };
	NNetModelReaderInterface const * m_pNMRI                 { nullptr };
	WinManager                     * m_pWinManager           { nullptr };
};
