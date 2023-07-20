// ScaleMenu.ixx
//
// Win32_utilities

module;

#include <Windows.h>

export module ScaleMenu;

import Win32_Util;
import Win32_Util_Resource;

export class ScaleMenu
{
public:

    void AppendScaleMenu(HMENU hMenu, LPCTSTR const title) const
    {
        HMENU hMenuPopup = Util::PopupMenu(hMenu, title);
        Util::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_OFF,       L"o&ff");
        Util::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_ON,        L"o&n");
        Util::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_GRID,      L"&grid");
        //Util::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_SNAP2GRID, L"&snap to grid");
        Util::Enable(hMenuPopup, IDM_SCALE_OFF,       m_uiState != IDM_SCALE_OFF);
        Util::Enable(hMenuPopup, IDM_SCALE_ON,        m_uiState != IDM_SCALE_ON);
        Util::Enable(hMenuPopup, IDM_SCALE_GRID,      m_uiState != IDM_SCALE_GRID);
        //Util::Enable(hMenuPopup, IDM_SCALE_SNAP2GRID, m_uiState != IDM_SCALE_SNAP2GRID);
    }

    unsigned int SetState(unsigned int const uiState)
    {
        unsigned int uiOldVal = m_uiState;
        m_uiState = uiState;
        return uiOldVal;
    }

    unsigned int GetState() const { return m_uiState; }

    bool IsScaleVisible   () const { return m_uiState >= IDM_SCALE_ON; }
    bool IsGridVisible    () const { return m_uiState >= IDM_SCALE_GRID; }
    //bool IsSnap2GridActive() const { return m_uiState == IDM_SCALE_SNAP2GRID; }

private:
    unsigned int m_uiState { IDM_SCALE_OFF };
};
