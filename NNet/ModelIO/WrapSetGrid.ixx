// WrapSetGrid.ixx
//
// ModelIO

module;

#include <iostream>
#include <string>

export module WrapSetGrid;

import Wrapper;
import BoolWrapper;
import BaseWindow;
import WinManager;

using std::wostream;
using std::wstring;
using std::endl;

export class WrapSetGrid : public Wrapper
{
public:
    WrapSetGrid()
     : Wrapper(NAME)
    {}

    static void WriteSetGrid
    (
        wostream        & out,
        BaseWindow const& baseWin,
        bool       const  bActive
    )
    {
        out << NAME << SPACE << WinManager::GetWindowName(baseWin) << PrefOnOff(bActive) << SPACE; // << endl;
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
                if (elem.m_pBaseWindow && elem.m_pBaseWindow->HasGrid())
                {
                    WriteSetGrid(out, *elem.m_pBaseWindow, true);
                }
            }
        );
    }

private:
    inline static const wstring NAME { L"SetGrid" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
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
    } m_wrapper { NAME };
};
