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

import IoUtil;
import Win32_Util;
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

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

class NNetWrapBase : public WrapBase
{
public:
    NNetWrapBase
    (
        wstring const& wstrName,
        NNetPreferences& pref
    )
      : WrapBase(wstrName),
        m_pref(pref)
    {}

protected:
    NNetPreferences& m_pref;
};

class WrapShowScales : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.SetScales(script.ScrReadBool(), false);
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ScalesVisible() ? PREF_ON : PREF_OFF);
    }
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
        out << (m_pref.ArrowsVisible() ? PREF_ON : PREF_OFF);
    }
};

class WrapShowSensorPoints: public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script & script) const final
    {
        m_pref.SetSensorPoints(script.ScrReadBool());
    }

    void Write(wostream & out) const final
    {
        out << (m_pref.SensorPointsVisible() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetPerfMonMode: public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script & script) const final
    {
        BaseWindow::SetPerfMonMode(script.ScrReadBool());
    }

    void Write(wostream & out) const final
    {
        out << (BaseWindow::PerfMonMode() ? PREF_ON : PREF_OFF);
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

class WrapSetSound : public NNetWrapBase
{
public:
    using NNetWrapBase::NNetWrapBase;

    void operator() (Script& script) const final
    {
        bool bMode { script.ScrReadBool() };
        if (bMode)
            m_pref.GetSound().On();
        else
            m_pref.GetSound().Off();
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.GetSound().IsOn() ? PREF_ON : PREF_OFF);
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

class WrapColor : public ScriptFunctor
{
public:
    WrapColor(WinManager &winMan)
      : m_winMan(winMan)
    {}

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

protected:
    WinManager & m_winMan;
};

void NNetPreferences::Initialize
(
    Sound       & sound,
    NNetModelIO & modelIO,
    WinManager &  winMan,
    HWND          hwndApp
)
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt", &sound);

    m_pSound      = &sound;
    m_pModelIO    = &modelIO;
    m_pWinManager = &winMan;
    m_hwndApp     = hwndApp;

    Add<WrapReadModel            >(L"ReadModel");
    Add<WrapSetPerfMonMode       >(L"SetPerfMonMode");
    Add<WrapInputCablesVisibility>(L"InputCablesVisibility");
    Add<WrapShowScales           >(L"ShowScales");
    Add<WrapShowArrows           >(L"ShowArrows");
    Add<WrapShowSensorPoints     >(L"ShowSensorPoints");
    Add<WrapSetSound             >(L"SetSound");

    SymbolTable::ScrDefConst(L"SetBKColor", new WrapColor(*m_pWinManager));
}

void NNetPreferences::WriteAppPreferences(wostream & out) const
{
    m_pWinManager->Apply2All
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

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}

void NNetPreferences::SetArrows(bool const bOn, bool const bAnimation)
{
    m_bArrows = bOn;
    SendMessage(m_hwndApp, WM_COMMAND, IDD_ARROWS, bAnimation);
}

void NNetPreferences::SetScales(bool const bOn, bool const bAnimation)
{
    m_bScales = bOn;
    SendMessage(m_hwndApp, WM_COMMAND, IDD_SCALES, bAnimation);
}
