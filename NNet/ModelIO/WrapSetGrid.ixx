// WrapSetGrid.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>
#include "Resource.h"

export module WrapSetGrid;

import Win32_Util_Resource;
import NNetWrapBase;
import WinManager;

using std::wostream;
using std::wstring;

export class WrapSetGrid : public NNetWrapBase
{
public:
    WrapSetGrid
    (
        wstring const& wstrName,
        NNetPreferences& pref
    )
        : NNetWrapBase(wstrName, pref)
    {}

    void operator() (Script& script) const final
    {
        unsigned int const uiWinId { script.ScrReadUint() };
        bool         const bActive { script.ScrReadBool() };
        BaseWindow* const pBaseWin { WinManager::GetBaseWindow(RootWinId(uiWinId)) };
        if (pBaseWin)
            pBaseWin->SetGrid(bActive, false);
        else
        {
            //Todo Error message
        }
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
        out << wstrWindow;
        PrefOnOff(out, pBaseWin->HasGrid());
    }
};
