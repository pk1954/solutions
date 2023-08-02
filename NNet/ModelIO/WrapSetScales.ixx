// WrapSetScales.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>

export module WrapSetScales;

import Win32_Util_Resource;
import Script;
import WrapBaseBool;
import BaseWindow;
import WinManager;

using std::wostream;
using std::wstring;

export class SetScalesFunctor : public ScriptFunctor
{
public:
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
};

export class WrapSetScales : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        SetScalesFunctor()(script);
    }

    void Write(wostream& out) const final
    {
        RootWinId  const  id         { RootWinId(IDM_MAIN_WINDOW) };
        wstring    const& wstrWindow { WinManager::GetWindowName(id) };
        BaseWindow const* pBaseWin   { WinManager::GetBaseWindow(id) };
        WriteCmdName(out);
        out << wstrWindow << PrefOnOff(pBaseWin->HasScales());
    }
};
