// WrapSetGrid.ixx
//
// Toolbox\Win32_utilities

module;

#include <iostream>
#include <string>

export module WrapSetGrid;

import Commands;
import RootWindow;
import WinManager;

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
                if (elem.m_pRootWindow && elem.m_pRootWindow->HasGrid())
                {
                    WriteSetGrid(out, *elem.m_pRootWindow, true);
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
            RootWindow * const pRootWin { WinManager::GetRootWindow(RootWinId(uiWinId)) };
            if (pRootWin)
                pRootWin->SetGrid(bActive, false);
            else
            {
                //TODO: Error message
            }
        }
    } m_wrapper { NAME };
};
