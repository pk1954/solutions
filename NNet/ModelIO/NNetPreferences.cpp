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

module NNetPreferences;

import BoolType;
import Commands;
import IoUtil;
import IoConstants;
import Win32_Util;
import Win32_Util_Resource;
import RunTime;
import NNetModelIO;
import WrapSetScales;
import WrapSetGrid;
import WrapColorLUT;
import Resource;

using std::wofstream;
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
    WrapReadModel(bool const bViewerMode)
        : Wrapper(NAME),
          m_bViewerMode(bViewerMode)
    {}

    void operator() (Script& script) const final
    {
        if (Preferences::m_bAutoOpen.Get() && !m_bViewerMode)
        {
            NNetModelIO::SetModelFileName(script.ScrReadString());
            WinManager::PostCommand(RootWinId(IDM_APPL_WINDOW), IDM_IMPORT_MODEL);
        }
    }

    void Write(wostream& out) const final
    {
        out << NAME << SPACE << Quoted(NNetPreferences::GetModelInterface()->GetModelFilePath());
    }

    inline static const wstring NAME { L"ReadModel" };

    bool m_bViewerMode;
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

void NNetPreferences::Initialize(bool const bViewerMode)
{
    Preferences::Initialize(L"NNetSimu_UserPreferences.txt");

    Preferences::AddWrapper(make_unique<WrapReadModel>(bViewerMode));
    Preferences::AddWrapper(make_unique<WrapInputCablesVisibility>());
   // Preferences::AddWrapper(make_unique<WrapColor>());
    Preferences::AddWrapper(make_unique<WrapSetScales>());
    Preferences::AddWrapper(make_unique<WrapSetGrid>());
    Preferences::AddWrapper(make_unique<WrapColorLUT>(NNetPreferences::m_colorLutScan));

    Preferences::AddBoolWrapper(L"ShowArrows",         m_bArrows);
    Preferences::AddBoolWrapper(L"ShowSensorPoints",   m_bSensorPoints);
    Preferences::AddBoolWrapper(L"SetPerfMonMode",     BaseWindow::m_bPerfMonMode);
    Preferences::AddBoolWrapper(L"ApplyFilter",        NNetPreferences::m_bFilter);
    Preferences::AddBoolWrapper(L"ScanAreaVisibility", NNetPreferences::m_bScanArea);
    Preferences::AddBoolWrapper(L"ModelFront",         NNetPreferences::m_bModelFront);
    Preferences::AddBoolWrapper(L"AskNotUndoable",     NNetPreferences::m_bAskNotUndoable);

    m_colorLutScan.AddBasePoint(  0, Color(D2D1::ColorF::Black));
    m_colorLutScan.AddBasePoint( 10, Color(D2D1::ColorF::Blue));
    m_colorLutScan.AddBasePoint(255, Color(D2D1::ColorF::Red));
    m_colorLutScan.Construct();

    m_colorLutVoltage.AddBasePoint(  0, Color(D2D1::ColorF::Black));
    m_colorLutVoltage.AddBasePoint(255, Color(D2D1::ColorF::Red));
    m_colorLutVoltage.Construct();
}

void NNetPreferences::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
{
    m_pNMRI = pNMRI;
}
