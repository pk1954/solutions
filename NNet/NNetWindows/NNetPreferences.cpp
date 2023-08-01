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
import WrapBase;
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
        NNetPreferences& pref
    )
        : WrapBase(wstrName),
        m_pref(pref)
    {}

protected:
    NNetPreferences& m_pref;
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
        WriteCmdName(out);
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
        WinManager::PostCommand(RootWinId(IDM_APPL_WINDOW), IDM_IMPORT_MODEL);
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
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
        BaseWindow* pBaseWin { WinManager::GetBaseWindow(id) };
        if (pBaseWin)
            pBaseWin->SetBackgroundColorRef(RGB(usRed, usGreen, usBlue));
        else
        {
            //TODO: Error message
        }
    }

    void Write(wostream& out) const final
    {
        WinManager::Apply2All
        (
            [this, &out](RootWinId const id, WinManager::MAP_ELEMENT const& elem)
            {
                if (elem.m_pBaseWindow)
                {
                    COLORREF col { elem.m_pBaseWindow->GetBackgroundColorRef() };
                    WriteCmdName(out);
                    out << elem.m_wstr << SPACE
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
        unsigned int const uiWinId  { script.ScrReadUint() };
        bool         const bActive  { script.ScrReadBool() };
        BaseWindow * const pBaseWin { WinManager::GetBaseWindow(RootWinId(uiWinId)) };
        if (pBaseWin)
            pBaseWin->SetScales(bActive, false);
        else
        {
            //TODO: Error message
        }
    }

    void Write(wostream& out) const final
    {
        RootWinId  const  idWinId    { RootWinId(IDM_MAIN_WINDOW) };
        wstring    const& wstrWindow { WinManager::GetWindowName(idWinId) };
        BaseWindow const* pBaseWin   { WinManager::GetBaseWindow(idWinId) };
        MainWindow const* pMainWin   { static_cast<MainWindow const*>(pBaseWin) };
        WriteCmdName(out);
        out << wstrWindow;
        PrefOnOff(out, pBaseWin->HasScales());
    }
};

class WrapShowGrid : public NNetWrapBase
{
public:
    WrapShowGrid
    (
        wstring const& wstrName,
        NNetPreferences& pref,
        unsigned int uiWinId
    )
      : NNetWrapBase(wstrName, pref),
        m_idWin(RootWinId(uiWinId))
    {}

    void operator() (Script& script) const final
    {
        unsigned int const uiWinId  { script.ScrReadUint() };
        bool         const bActive  { script.ScrReadBool() };
        BaseWindow * const pBaseWin { WinManager::GetBaseWindow(RootWinId(uiWinId)) };
        if (pBaseWin)
            pBaseWin->SetGrid(bActive, false);
        else
        {
            //Todo Error message
        }
    }

    void Write(wostream& out) const final
    {
        wstring    const& wstrWindow { WinManager::GetWindowName(m_idWin) };
        BaseWindow const* pBaseWin   { WinManager::GetBaseWindow(m_idWin) };
        WriteCmdName(out);
        out << wstrWindow;
        PrefOnOff(out, pBaseWin->HasGrid());
    }

private:
    RootWinId const m_idWin;
};

void NNetPreferences::Initialize(NNetModelIO & modelIO)
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt");

    m_pModelIO = &modelIO;

    AddNNetPrefRapper<WrapReadModel            >(L"ReadModel");
    AddNNetPrefRapper<WrapInputCablesVisibility>(L"InputCablesVisibility");
    AddNNetPrefRapper<WrapShowScales           >(L"ShowScales");
    AddNNetPrefRapper<WrapColor                >(L"SetBKColor");

    AddBoolWrapper(L"ShowArrows",       m_bArrows);
    AddBoolWrapper(L"ShowSensorPoints", m_bSensorPoints);
    AddBoolWrapper(L"SetPerfMonMode",   BaseWindow::m_bPerfMonMode);

    AddWrapper(make_unique<WrapShowGrid>(L"ShowGrid", *this, IDM_MAIN_WINDOW));
    AddWrapper(make_unique<WrapShowGrid>(L"ShowGrid", *this, IDM_SIG_DESIGNER));
}

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}
