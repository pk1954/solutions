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
import BoolWrapper;
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
import WrapColorLUT;

using std::wofstream;
using std::wcout;
using std::endl;
using std::filesystem::exists;
using std::filesystem::path;
using std::make_unique;

class WrapFilter : public Wrapper
{
public:
    WrapFilter()
        : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetPreferences::m_bFilter.Set(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << PrefOnOff(NNetPreferences::m_bFilter.Get());
    }

    inline static const wstring NAME { L"ApplyFilter" };
};

class WrapScanAreaVisibility : public Wrapper
{
public:
    WrapScanAreaVisibility()
        : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetPreferences::m_bScanArea.Set(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << PrefOnOff(NNetPreferences::m_bScanArea.Get());
    }

    inline static const wstring NAME { L"ScanAreaVisibility" };
};

class WrapModelFront : public Wrapper
{
public:
    WrapModelFront()
        : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetPreferences::m_bModelFront.Set(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << PrefOnOff(NNetPreferences::m_bModelFront.Get());
    }

    inline static const wstring NAME { L"ModelFront" };
};

class WrapInputCablesVisibility : public Wrapper
{
public:
    WrapInputCablesVisibility()
        : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetPreferences::m_bInputCables.SetVisibility(static_cast<ShowInputCables::tVisibility>(script.ScrReadInt()));
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << static_cast<int>(NNetPreferences::m_bInputCables.Visibility());
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
        if (Preferences::m_bAutoOpen.Get())
        {
            NNetModelIO::SetModelFileName(script.ScrReadString());
            WinManager::PostCommand(RootWinId(IDM_APPL_WINDOW), IDM_IMPORT_MODEL);
        }
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << DOUBLE_QUOTE << NNetPreferences::GetModelInterface()->GetModelFilePath() << DOUBLE_QUOTE;
    }

    inline static const wstring NAME { L"ReadModel" };
};

//class WrapColor : public Wrapper
//{
//public:
//    WrapColor()
//        : Wrapper(NAME)
//    {}
//
//    void operator() (Script& script) const final
//    {
//        RootWinId id  { script.ScrReadInt() };
//        COLORREF  col { ScrReadColorRef(script) };
//        if (RootWindow* pRootWin { WinManager::GetRootWindow(id) })
//            pRootWin->SetBackgroundColorRef(col);
//        else
//        {
//            //TODO: Error message
//        }
//    }
//
//    void Write(wostream& out) const final
//    {
//        WinManager::Apply2All
//        (
//            [this, &out](RootWinId const id, WinManager::MAP_ELEMENT const& elem)
//            {
//                if (elem.m_pRootWindow)
//                    WriteBackgroundColor(out, elem);
//            }
//        );
//    }
//
//private:
//
//    inline static const wstring NAME { L"SetBKColor" };
//
//    static void WriteBackgroundColor(wostream &out, WinManager::MAP_ELEMENT const& elem)
//    {
//        out << NAME        << SPACE 
//            << elem.m_wstr << SPACE 
//            << elem.m_pRootWindow->GetBackgroundColorRef() 
//            << endl;
//    }
//};

void NNetPreferences::Initialize()
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt");

    Preferences::AddWrapper(make_unique<WrapReadModel>());
    Preferences::AddWrapper(make_unique<WrapInputCablesVisibility>());
    Preferences::AddWrapper(make_unique<WrapScanAreaVisibility>());
    Preferences::AddWrapper(make_unique<WrapModelFront>());
    Preferences::AddWrapper(make_unique<WrapFilter>());
   // Preferences::AddWrapper(make_unique<WrapColor>());
    Preferences::AddWrapper(make_unique<WrapSetScales>());
    Preferences::AddWrapper(make_unique<WrapSetGrid>());
    Preferences::AddWrapper(make_unique<WrapColorLUT>(NNetPreferences::m_colorLutScan));

    Preferences::AddBoolWrapper(L"ShowArrows",       m_bArrows);
    Preferences::AddBoolWrapper(L"ShowSensorPoints", m_bSensorPoints);
    Preferences::AddBoolWrapper(L"SetPerfMonMode",   BaseWindow::m_bPerfMonMode);

    m_colorLutScan.AddBasePoint(  0, Color(D2D1::ColorF::Black));
    m_colorLutScan.AddBasePoint( 10, Color(D2D1::ColorF::Blue));
    m_colorLutScan.AddBasePoint(255, Color(D2D1::ColorF::Red));
    m_colorLutScan.Construct();

    m_colorLutVoltage.AddBasePoint(  0, Color(D2D1::ColorF::Black));
    m_colorLutVoltage.AddBasePoint(255, Color(D2D1::ColorF::Red));
    m_colorLutVoltage.Construct();

}

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const* pNMRI)
{
    m_pNMRI = pNMRI;
}
