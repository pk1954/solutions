// WrapSetScales.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>

export module WrapSetScales;

import IoConstants;
import Script;
import WrapBaseBool;
import BaseWindow;
import WinManager;

using std::wostream;
using std::wstring;
using std::endl;

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
        WinManager::Apply2All
        (
            [this, &out](RootWinId const id, WinManager::MAP_ELEMENT const& elem) 
            { 
                if (elem.m_pBaseWindow && elem.m_pBaseWindow->HasScales())
                {
                    out << GetName() << SPACE << elem.m_wstr << PrefOnOff(true) << endl;
                }
            }
        );
    }
};
