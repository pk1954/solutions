// WrapSetGrid.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>
#include "Resource.h"

export module WrapSetGrid;

import Win32_Util_Resource;
import Script;
import WrapBaseBool;
import BaseWindow;
import WrapBase;
import WinManager;

using std::wostream;
using std::wstring;
using std::endl;

export class SetGridFunctor : public ScriptFunctor
{
public:
    void operator() (Script& script) const final
    {
        unsigned int const uiWinId  { script.ScrReadUint() };
        bool         const bActive  { script.ScrReadBool() };
        BaseWindow * const pBaseWin { WinManager::GetBaseWindow(RootWinId(uiWinId)) };
        if (pBaseWin)
            pBaseWin->SetGrid(bActive, false);
        else
        {
            //TODO: Error message
        }
    }
};

export class WrapSetGrid : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        SetGridFunctor()(script);
    }

    void Write(wostream& out) const final
    {
        WriteWinGridState(out, IDM_MAIN_WINDOW);
        WriteWinGridState(out, IDM_SIG_DESIGNER);
    }

    void WriteWinGridState(wostream& out, unsigned int const uiWinId) const
    {
        RootWinId  const  idWin      { RootWinId(uiWinId) };
        wstring    const& wstrWindow { WinManager::GetWindowName(idWin) };
        BaseWindow const* pBaseWin   { WinManager::GetBaseWindow(idWin) };
        WriteCmdName(out);
        out << wstrWindow << PrefOnOff(pBaseWin->HasGrid()) << endl;
    }
};
