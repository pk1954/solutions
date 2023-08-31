// WrapSetScales.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>

export module WrapSetScales;

import Wrapper;
import BoolWrapper;
import RootWindow;
import WinManager;

using std::wostream;
using std::wstring;
using std::endl;

export class WrapSetScales : public Wrapper
{
public:
    WrapSetScales()
        : Wrapper(NAME)
    {}

    static void WriteSetScales
    (
        wostream        & out,
        RootWindow const& rootWin,
        bool       const  bActive
    )
    {
        out << NAME << SPACE << WinManager::GetWindowName(rootWin) << PrefOnOff(bActive) << SPACE; // << endl;
    }

    void operator() (Script& script) const final
    {
        m_wrapper(script);
    }

    void Write(wostream& out) const final
    {
        WinManager::Apply2All
        (
            [this, &out](RootWinId const id, WinManager::MAP_ELEMENT const& elem) 
            { 
                if (elem.m_pRootWindow && elem.m_pRootWindow->HasScales())
                {
                    WriteSetScales(out, *elem.m_pRootWindow, true);
                }
            }
        );
    }

private:
    inline static const wstring NAME { L"SetScales" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            unsigned int const uiWinId { script.ScrReadUint() };
            bool         const bActive { script.ScrReadBool() };
            RootWindow* const pBaseWin { WinManager::GetRootWindow(RootWinId(uiWinId)) };
            if (pBaseWin)
                pBaseWin->SetScales(bActive, false);
            else
            {
                //TODO: Error message
            }
        }
    } m_wrapper { NAME };
};
