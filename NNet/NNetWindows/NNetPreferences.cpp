// NNetPreferences.cpp
//
// NNetWindows

module;

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:NNetPreferences;

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
        wstring wstrFile { script.ScrReadString() };
        if (!m_pref.GetModelIO().Import(wstrFile, NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL)))
        {
            SendMessage(m_pref.GetHwndApp(), WM_COMMAND, IDM_NEW_MODEL, 0);
        }
    }

    void Write(wostream& out) const final
    {
        out << L"\"" << m_pref.GetModelInterface()->GetModelFilePath() << L"\"";
    }
};

void NNetPreferences::Initialize
(
    Sound       & sound,
    NNetModelIO & modelIO,
    HWND          hwndApp
)
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt", &sound);

    m_pSound   = &sound;
    m_pModelIO = &modelIO;
    m_hwndApp  = hwndApp;

    Add<WrapReadModel            >(L"ReadModel");
    Add<WrapSetPerfMonMode       >(L"SetPerfMonMode");
    Add<WrapInputCablesVisibility>(L"InputCablesVisibility");
    Add<WrapShowScales           >(L"ShowScales");
    Add<WrapShowArrows           >(L"ShowArrows");
    Add<WrapShowSensorPoints     >(L"ShowSensorPoints");
    Add<WrapSetSound             >(L"SetSound");
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
