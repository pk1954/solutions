// ScaleMenu.ixx
//
// Win32_utilities

module;

#include <cassert>
#include <Windows.h>

export module ScaleMenu;

import Win32_Util;
import Win32_Util_Resource;

export class ScaleMenu
{
public:

    void AppendScaleMenu
    (
        HMENU         hMenu, 
        LPCTSTR const title,
        bool    const bScale,
        bool    const bGrid
    ) const
    {
        HMENU hMenuPopup = ::PopupMenu(hMenu, title);
        ::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_OFF,  L"o&ff");
        ::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_ON,   L"o&n");
        ::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_GRID, L"&grid");
        ::Enable(hMenuPopup, IDM_SCALE_OFF,  bScale);
        ::Enable(hMenuPopup, IDM_SCALE_ON,   bGrid || !bScale);
        ::Enable(hMenuPopup, IDM_SCALE_GRID, !bGrid);
    }

};
