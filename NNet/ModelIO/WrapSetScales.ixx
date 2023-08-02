// WrapSetScales.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>

export module WrapSetScales;

import Win32_Util_Resource;
import NNetWrapBase;
import WinManager;

using std::wostream;
using std::wstring;

export class WrapSetScales : public NNetWrapBase
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
        WriteCmdName(out);
        out << wstrWindow;
        PrefOnOff(out, pBaseWin->HasScales());
    }
};
