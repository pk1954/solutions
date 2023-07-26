// NNetPreferences.cpp
//
// NNetWindows

module;

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:NNetPreferences;

import BoolType;
import IoUtil;
import Win32_Util;
import Win32_Util_Resource;
import Script;
import Symtab;
import NNetModelIO;
import :DescriptionWindow;
import :NNetInputOutputUI;
import :MainWindow;

using std::wofstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;
using std::filesystem::exists;
using std::filesystem::path;
using std::make_unique;

class NNetWrapBase : public WrapBase
{
public:
    NNetWrapBase
    (
        wstring const& wstrName,
        NNetPreferences& pref,
        WinManager& winMan
    )
        : WrapBase(wstrName),
        m_pref(pref),
        m_winMan(winMan)
    {}

protected:
    NNetPreferences& m_pref;
    WinManager& m_winMan;
};

class WrapBaseBool : public WrapBase
{
public:
    WrapBaseBool
    (
        wstring const& wstrName,
        BoolType     & boolType
    )
        : WrapBase(wstrName),
          m_boolType(boolType)
    {}

    void operator() (Script& script) const final
    {
        m_boolType.Set(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        PrefOnOff(out, m_boolType.Get());
    }

protected:
    BoolType& m_boolType;
};

class WrapShowArrows : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.SetArrows(script.ScrReadBool(), false);
    }

    void Write(wostream& out) const final
    {
        PrefOnOff(out, m_pref.ArrowsVisible());
    }
};

class WrapInputCablesVisibility : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.SetInputCablesVisibility(static_cast<NNetPreferences::tInputCablesVisibility>(script.ScrReadInt()));
    }

    void Write(wostream& out) const final
    {
        out << static_cast<int>(m_pref.InputCablesVisibility());
    }
};

class WrapReadModel : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.GetModelIO().SetModelFileName(script.ScrReadString());
        PostMessage(m_pref.GetHwndApp(), WM_COMMAND, IDM_IMPORT_MODEL, 0);
    }

    void Write(wostream& out) const final
    {
        out << DOUBLE_QUOTE << m_pref.GetModelInterface()->GetModelFilePath() << DOUBLE_QUOTE;
    }
};

class WrapColor : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        RootWinId id(script.ScrReadInt());
        script.ScrReadString(L"RGB");
        script.ScrReadSpecial(OPEN_BRACKET);
        unsigned short usRed   { script.ScrReadUshort() };
        script.ScrReadSpecial(ID_SEPARATOR);
        unsigned short usGreen { script.ScrReadUshort() };
        script.ScrReadSpecial(ID_SEPARATOR);
        unsigned short usBlue  { script.ScrReadUshort() };
        script.ScrReadSpecial(CLOSE_BRACKET);
        COLORREF          col { RGB(usRed, usGreen, usBlue) };
        BaseWindow * pBaseWin { m_winMan.GetBaseWindow(id) };
        pBaseWin->SetBackgroundColorRef(col);
    }

    void Write(wostream& out) const final
    {
        m_winMan.Apply2All
        (
            [this, &out](RootWinId const id, WinManager::MAP_ELEMENT const& elem)
            {
                if (elem.m_pBaseWindow)
                {
                    COLORREF col { elem.m_pBaseWindow->GetBackgroundColorRef() };
                    out << L"SetBKColor" << SPACE
                        << elem.m_wstr << SPACE
                        << L"RGB"
                        << OPEN_BRACKET
                        << GetRValue(col)
                        << ID_SEPARATOR
                        << GetGValue(col)
                        << ID_SEPARATOR
                        << GetBValue(col)
                        << CLOSE_BRACKET
                        << endl;
                }
            }
        );
    }
};

class WrapShowScales : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        unsigned int uiWinId { script.ScrReadUint() };
        unsigned int uiState { script.ScrReadUint() };
        BaseWindow *pBaseWin { m_winMan.GetBaseWindow(RootWinId(uiWinId)) };
        pBaseWin->SendCommand(uiWinId, false);
    }

    void Write(wostream& out) const final
    {
        RootWinId  const   idWinId  { RootWinId(IDM_MAIN_WINDOW) };
        wstring    const& wstrName { m_winMan.GetWindowName(idWinId) };
        BaseWindow const* pBaseWin { m_winMan.GetBaseWindow(idWinId) };
        MainWindow const* pMainWin { static_cast<MainWindow const *>(pBaseWin) };

        out << wstrName << SPACE << pMainWin->GetScaleMode() << endl;
    }
};

void NNetPreferences::AddBool(wstring const& name, BoolType& boolType)
{
    AddWrapper(make_unique<WrapBaseBool>(name, boolType));
}

void NNetPreferences::Initialize
(
    Sound       & sound,
    NNetModelIO & modelIO,
    WinManager  & winMan,
    MainWindow  & mainWin,
    HWND          hwndApp
)
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt", &sound);

    m_pSound      = &sound;
    m_pModelIO    = &modelIO;
    m_pWinManager = &winMan;
    m_hwndApp     = hwndApp;

    Add<WrapReadModel            >(L"ReadModel");
    Add<WrapInputCablesVisibility>(L"InputCablesVisibility");
    Add<WrapShowScales           >(L"ShowScales");
    Add<WrapShowArrows           >(L"ShowArrows");
    Add<WrapColor                >(L"SetBKColor");

    AddBool(L"ShowSensorPoints", m_bSensorPoints);
    AddBool(L"SetPerfMonMode", BaseWindow::m_bPerfMonMode);
}

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}

void NNetPreferences::SetArrows(bool const bOn, bool const bAnimation)
{
    m_bArrows.Set(bOn);
    m_pWinManager->SendCommand(RootWinId(IDM_MAIN_WINDOW), IDD_ARROW_ANIMATION, bAnimation);
}
