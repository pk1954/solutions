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

import ErrHndl;
import SoundInterface;
import ScriptFile;
import BaseWindow;
import Win32_Util;
import Symtab;
import AutoOpen;
import Script;
import NNetModelIO;
import NNetModel;
import :DescriptionWindow;
import :NNetInputOutputUI;
import :NNetAppMenu;
import :MainWindow;

using std::make_unique;
using std::wofstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;
using std::filesystem::exists;
using std::filesystem::path;

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

class WrapShowScales : public WrapBase
{
public:
    explicit WrapShowScales(Preferences& pref)
      : WrapBase(L"ShowScales"),
        m_pref(pref)
    {}

    void operator() (Script& script) const final
    {
        m_pref.SetScales(static_cast<bool>(script.ScrReadUint()));
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ScalesVisible() ? PREF_ON : PREF_OFF);
    }

private:
    Preferences& m_pref;
};

class WrapShowArrows : public WrapBase
{
public:
    explicit WrapShowArrows(Preferences& pref)
      : WrapBase(L"ShowArrows"),
        m_pref(pref)
    {}

    void operator() (Script& script) const final
    {
        m_pref.SetArrows(static_cast<bool>(script.ScrReadUint()));
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ArrowsVisible() ? PREF_ON : PREF_OFF);
    }

private:
    Preferences& m_pref;
};

class WrapShowSensorPoints: public WrapBase
{
public:
    explicit WrapShowSensorPoints(Preferences & pref)
      : WrapBase(L"ShowSensorPoints"),
        m_pref(pref)
    {}

    void operator() (Script & script) const final
    {
        m_pref.SetSensorPoints(static_cast<bool>(script.ScrReadUint()));
    }

    void Write(wostream & out) const final
    {
        out << (m_pref.SensorPointsVisible() ? PREF_ON : PREF_OFF);
    }

private:
    Preferences & m_pref;
};

class WrapSetAutoOpen: public WrapBase
{
public:
    WrapSetAutoOpen()
        : WrapBase(L"SetAutoOpen")
    {}

    void operator() (Script & script) const final
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
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

class WrapSetPerfMonMode: public WrapBase
{
public:
    WrapSetPerfMonMode()
        : WrapBase(L"SetPerfMonMode")
    {}

    void operator() (Script & script) const final
    {
        BaseWindow::SetPerfMonMode(static_cast<bool>(script.ScrReadUint()));
    }

    void Write(wostream & out) const final
    {
        out << (BaseWindow::PerfMonMode() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetSound: public WrapBase
{
public:
    explicit WrapSetSound(Sound & sound)
      : WrapBase(L"SetSound"),
        m_sound(sound)
    {}

    void operator() (Script & script) const final
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if (bMode)
            m_sound.On();
        else
            m_sound.Off();
    }

    void Write(wostream & out) const final
    {
        out << (m_sound.IsOn() ? PREF_ON : PREF_OFF);
    }

private:
    Sound & m_sound;
};

class WrapDescWinFontSize : public WrapBase
{
public:
    explicit WrapDescWinFontSize(DescriptionWindow& descWin)
        : WrapBase(L"DescWinFontSize"),
        m_descWin(descWin)
    {}

    void operator() (Script& script) const final
    {
        m_descWin.SetFontSize(script.ScrReadInt());
    }

    void Write(wostream& out) const final
    {
        out << m_descWin.GetFontSize();
    }

private:
    DescriptionWindow& m_descWin;
};

class WrapInputCablesVisibility : public WrapBase
{
public:
    explicit WrapInputCablesVisibility(Preferences & pref)
      : WrapBase(L"InputCablesVisibility"),
        m_pref(pref)
    {}

    void operator() (Script& script) const final
    {
        m_pref.SetInputCablesVisibility(static_cast<Preferences::tInputCablesVisibility>(script.ScrReadInt()));
    }

    void Write(wostream& out) const final
    {
        out << static_cast<int>(m_pref.InputCablesVisibility());
    }

private:
    Preferences& m_pref;
};

class WrapReadModel: public WrapBase
{
public:
    WrapReadModel
    (
        Preferences const * pPref,
        NNetModelIO       & modelIO, 
        HWND        const   hwndApp
    )
    : WrapBase(L"ReadModel"),
      m_pPref(pPref),
      m_modelIO(modelIO),
      m_hwndApp(hwndApp)
    {}

    void operator() (Script & script) const final
    {
        wstring wstrFile { script.ScrReadString() };
        if (! m_modelIO.Import(wstrFile, NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL)))
        {
            SendMessage(m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0);
        }
     }

    void Write(wostream & out) const final
    {
        out << L"\"" << m_pPref->GetModelInterface()->GetModelFilePath() << L"\"";
    }

private:
    Preferences const * m_pPref;
    NNetModelIO       & m_modelIO;
    HWND                m_hwndApp;
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

    m_hwndApp = hwndApp;

    m_wstrPreferencesFile = Util::GetCurrentDir();
    m_wstrPreferencesFile += L"\\" + PREFERENCES_FILE_NAME;
    
    m_prefVector.push_back(make_unique<WrapShowScales>(*this));
    m_prefVector.push_back(make_unique<WrapShowArrows>(*this));
    m_prefVector.push_back(make_unique<WrapShowSensorPoints>(*this));
    m_prefVector.push_back(make_unique<WrapDescWinFontSize>(descWin));
    m_prefVector.push_back(make_unique<WrapSetAutoOpen>());
    m_prefVector.push_back(make_unique<WrapSetSound>(sound));
    m_prefVector.push_back(make_unique<WrapReadModel>(this, modelIO, hwndApp));
    m_prefVector.push_back(make_unique<WrapSetPerfMonMode>());
    m_prefVector.push_back(make_unique<WrapInputCablesVisibility>(*this));

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
