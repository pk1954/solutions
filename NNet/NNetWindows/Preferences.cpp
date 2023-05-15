// Preferences.cpp
//
// NNetWindows

module;

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:Preferences;

import SoundInterface;
import Win32_Util;
import AutoOpen;
import Script;
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

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

class PrefWrapBase : public WrapBase
{
public:
    PrefWrapBase
    (
        wstring const& wstrName, 
        Preferences  & pref
    )
      : WrapBase(wstrName),
        m_pref(pref)
    {}

protected:
    Preferences& m_pref;
};

class WrapShowScales : public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.SetScales(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ScalesVisible() ? PREF_ON : PREF_OFF);
    }
};

class WrapShowArrows : public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.SetArrows(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ArrowsVisible() ? PREF_ON : PREF_OFF);
    }
};

class WrapShowSensorPoints: public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script & script) const final
    {
        m_pref.SetSensorPoints(script.ScrReadBool());
    }

    void Write(wostream & out) const final
    {
        out << (m_pref.SensorPointsVisible() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetAutoOpen: public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script & script) const final
    {
        bool bMode { script.ScrReadBool() };
        if (bMode)
            AutoOpen::On();
        else
            AutoOpen::Off();
    }

    void Write(wostream & out) const final
    {
        out << (AutoOpen::IsOn() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetPerfMonMode: public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script & script) const final
    {
        BaseWindow::SetPerfMonMode(script.ScrReadBool());
    }

    void Write(wostream & out) const final
    {
        out << (BaseWindow::PerfMonMode() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetSound: public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script & script) const final
    {
        bool bMode { script.ScrReadBool() };
        if (bMode)
            m_pref.GetSound().On();
        else
            m_pref.GetSound().Off();
    }

    void Write(wostream & out) const final
    {
        out << (m_pref.GetSound().IsOn() ? PREF_ON : PREF_OFF);
    }
};

class WrapDescWinFontSize : public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.GetDescWin().SetFontSize(script.ScrReadInt());
    }

    void Write(wostream& out) const final
    {
        out << m_pref.GetDescWin().GetFontSize();
    }
};

class WrapInputCablesVisibility : public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script& script) const final
    {
        m_pref.SetInputCablesVisibility(static_cast<Preferences::tInputCablesVisibility>(script.ScrReadInt()));
    }

    void Write(wostream& out) const final
    {
        out << static_cast<int>(m_pref.InputCablesVisibility());
    }
};

class WrapReadModel: public PrefWrapBase
{
public:
    using PrefWrapBase::PrefWrapBase;

    void operator() (Script & script) const final
    {
        wstring wstrFile { script.ScrReadString() };
        if (!m_pref.GetModelIO().Import(wstrFile, NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL)))
        {
            SendMessage(m_pref.GetHwndApp(), WM_COMMAND, IDM_NEW_MODEL, 0);
        }
     }

    void Write(wostream & out) const final
    {
        out << L"\"" << m_pref.GetModelInterface()->GetModelFilePath() << L"\"";
    }
};

void Preferences::Initialize
(
    DescriptionWindow & descWin,
    Sound             & sound, 
    NNetModelIO       & modelIO,
    HWND                hwndApp
)
{
    static wstring const PREFERENCES_FILE_NAME { L"NNetSimu_UserPreferences.txt" };

    m_hwndApp  = hwndApp;
    m_pSound   = &sound;
    m_pModelIO = &modelIO;
    m_pDescWin = &descWin;

    m_wstrPreferencesFile = Util::GetCurrentDir();
    m_wstrPreferencesFile += L"\\" + PREFERENCES_FILE_NAME;
    
    Add<WrapShowScales           >(L"ShowScales");
    Add<WrapShowArrows           >(L"ShowArrows");
    Add<WrapShowSensorPoints     >(L"ShowSensorPoints");
    Add<WrapDescWinFontSize      >(L"DescWinFontSize");
    Add<WrapSetAutoOpen          >(L"SetAutoOpen");
    Add<WrapSetSound             >(L"SetSound");
    Add<WrapReadModel            >(L"ReadModel");
    Add<WrapSetPerfMonMode       >(L"SetPerfMonMode");
    Add<WrapInputCablesVisibility>(L"InputCablesVisibility");

    SymbolTable::ScrDefConst(PREF_OFF, 0L);
    SymbolTable::ScrDefConst(PREF_ON,  1L);
}

bool Preferences::ReadPreferences() const
{
    if (exists(m_wstrPreferencesFile))
    {
        wcout << Scanner::COMMENT_START << L"Read preferences file " << m_wstrPreferencesFile << endl;
        Script script;
        script.SetEcho(false);
        return script.ScrProcess(m_wstrPreferencesFile);
    }
    else 
    {
        wcout << Scanner::COMMENT_SYMBOL << L" +++ Preferences file " << m_wstrPreferencesFile << L" not found" << endl;
        wcout << Scanner::COMMENT_SYMBOL << L" +++ Using defaults" << endl;
        return false;
    }
}

void Preferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}

void Preferences::SetArrows(bool const bOn)
{
    m_bArrows = bOn;
    SendMessage(m_hwndApp, WM_COMMAND, IDD_ARROWS, 0);
}

void Preferences::SetScales(bool const bOn)
{
    m_bScales = bOn;
    SendMessage(m_hwndApp, WM_COMMAND, IDD_SCALES, 0);
}

void Preferences::SetSensorPoints(bool const bOn)
{
    m_bSensorPoints = bOn;
    SendMessage(m_hwndApp, WM_COMMAND, IDD_SENSOR_PNTS, 0);
}

bool Preferences::WritePreferences() const
{
    wofstream prefFile(m_wstrPreferencesFile);
    for (auto const& it : m_prefVector)
    {
        it->WriteCmdName(prefFile);
        it->Write(prefFile);
        prefFile << endl;
    }
    prefFile.close();
    wcout << Scanner::COMMENT_START << L"preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
