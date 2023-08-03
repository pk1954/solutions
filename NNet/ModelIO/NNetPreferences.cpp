// NNetPreferences.cpp
//
// ModelIO

module;

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <Windows.h>
#include "Resource.h"

module NNetPreferences;

import BoolType;
import IoUtil;
import IoConstants;
import WrapBase;
import Win32_Util;
import Win32_Util_Resource;
import Script;
import Symtab;
import NNetModelIO;
import WrapSetScales;
import WrapSetGrid;

using std::wofstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;
using std::filesystem::exists;
using std::filesystem::path;
using std::make_unique;

class WrapInputCablesVisibility : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        NNetPreferences::SetInputCablesVisibility(static_cast<NNetPreferences::tInputCablesVisibility>(script.ScrReadInt()));
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << static_cast<int>(NNetPreferences::InputCablesVisibility());
    }
};

class WrapReadModel : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        NNetPreferences::GetModelIO().SetModelFileName(script.ScrReadString());
        WinManager::PostCommand(RootWinId(IDM_APPL_WINDOW), IDM_IMPORT_MODEL);
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << DOUBLE_QUOTE << NNetPreferences::GetModelInterface()->GetModelFilePath() << DOUBLE_QUOTE;
    }
};

class WrapColor : public WrapBase
{
public:
    using WrapBase::WrapBase;

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

void NNetPreferences::Initialize(NNetModelIO & modelIO)
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt");

    m_pModelIO = &modelIO;

    Preferences::AddWrapper(make_unique<WrapReadModel            >(L"ReadModel"));
    Preferences::AddWrapper(make_unique<WrapInputCablesVisibility>(L"InputCablesVisibility"));
    Preferences::AddWrapper(make_unique<WrapSetScales            >(L"SetScales"));
    Preferences::AddWrapper(make_unique<WrapColor                >(L"SetBKColor"));
    Preferences::AddWrapper(make_unique<WrapSetGrid              >());

    Preferences::AddBoolWrapper(L"ShowArrows",       m_bArrows);
    Preferences::AddBoolWrapper(L"ShowSensorPoints", m_bSensorPoints);
    Preferences::AddBoolWrapper(L"SetPerfMonMode",   BaseWindow::m_bPerfMonMode);
}

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}
