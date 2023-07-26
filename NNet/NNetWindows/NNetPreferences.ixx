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

class MainWindow;

export class NNetPreferences: public Preferences
{
public:
	void Initialize
	(
		Sound &, 
		NNetModelIO&,
		WinManager&,
		MainWindow&,
		HWND const
	);

	void SetModelInterface(NNetModelReaderInterface const *);

	void SetArrows(bool const, bool const);
	bool ArrowsVisible() const { return m_bArrows.Get(); }

	Sound& GetSound() { return *m_pSound; }

	enum class tInputCablesVisibility { all, nonStd, active, none };

	tInputCablesVisibility InputCablesVisibility() const { return m_inputCablesVisibility; }
	void SetInputCablesVisibility(tInputCablesVisibility v) { m_inputCablesVisibility = v; }

	NNetModelReaderInterface const *GetModelInterface() const { return m_pNMRI; };
	HWND                            GetHwndApp()        const { return m_hwndApp; }
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

	void AddBool(wstring const&, BoolType&);

	HWND                             m_hwndApp               { nullptr };
	NNetModelIO                    * m_pModelIO              { nullptr };
	tInputCablesVisibility           m_inputCablesVisibility { tInputCablesVisibility::nonStd };
	NNetModelReaderInterface const * m_pNMRI                 { nullptr };
	Sound                          * m_pSound                { nullptr };
	WinManager                     * m_pWinManager           { nullptr };
};
