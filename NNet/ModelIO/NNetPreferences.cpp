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
import Wrapper;
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

class WrapInputCablesVisibility : public Wrapper
{
public:
    WrapInputCablesVisibility()
        : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetPreferences::SetInputCablesVisibility(static_cast<NNetPreferences::tInputCablesVisibility>(script.ScrReadInt()));
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << static_cast<int>(NNetPreferences::InputCablesVisibility());
    }

    inline static const wstring NAME { L"InputCablesVisibility" };
};

class WrapReadModel : public Wrapper
{
public:
    WrapReadModel()
        : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetModelIO::SetModelFileName(script.ScrReadString());
        WinManager::PostCommand(RootWinId(IDM_APPL_WINDOW), IDM_IMPORT_MODEL);
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << DOUBLE_QUOTE << NNetPreferences::GetModelInterface()->GetModelFilePath() << DOUBLE_QUOTE;
    }

    inline static const wstring NAME { L"ReadModel" };
};

class WrapColor : public Wrapper
{
public:
    WrapColor()
        : Wrapper(NAME)
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
        RootWindow* pRootWin { WinManager::GetRootWindow(id) };
        if (pRootWin)
            pRootWin->SetBackgroundColorRef(RGB(usRed, usGreen, usBlue));
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
                if (elem.m_pRootWindow)
                    WriteBackgroundColor(out, elem);
            }
        );
    }

private:

    inline static const wstring NAME { L"SetBKColor" };

    static void WriteBackgroundColor(wostream &out, WinManager::MAP_ELEMENT const& elem)
    {
        COLORREF col { elem.m_pRootWindow->GetBackgroundColorRef() };
        out << NAME << SPACE << elem.m_wstr << SPACE
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
};

void NNetPreferences::Initialize()
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt");

    Preferences::AddWrapper(make_unique<WrapReadModel>());
    Preferences::AddWrapper(make_unique<WrapInputCablesVisibility>());
    Preferences::AddWrapper(make_unique<WrapColor>());
    Preferences::AddWrapper(make_unique<WrapSetScales>());
    Preferences::AddWrapper(make_unique<WrapSetGrid>());

    Preferences::AddBoolWrapper(L"ShowArrows",       m_bArrows);
    Preferences::AddBoolWrapper(L"ShowSensorPoints", m_bSensorPoints);
    Preferences::AddBoolWrapper(L"SetPerfMonMode",   BaseWindow::m_bPerfMonMode);
    Preferences::AddBoolWrapper(L"SetScanMode",      m_bScanMode);
}

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}
