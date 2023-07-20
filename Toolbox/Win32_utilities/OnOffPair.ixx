// OnOffPair.ixx
//
// Win32_utilities

module;

#include <Windows.h>

export module OnOffPair;

import Win32_Util;

export class OnOffPair
{
public:
    OnOffPair
    (
        unsigned int const uiOn,
        unsigned int const uiOff
    )
      : m_uiOn(uiOn),
        m_uiOff(uiOff)
    { }

    void AppendOnOffMenu(HMENU hMenu, LPCTSTR const title) const
    {
        HMENU hMenuPopup = Util::PopupMenu(hMenu, title);
        Util::AddMenu(hMenuPopup, MF_STRING, m_uiOn,  L"o&n");
        Util::AddMenu(hMenuPopup, MF_STRING, m_uiOff, L"o&ff");
    }

    void EnableOnOff(HMENU hMenu, bool const bCrit)
    {
        Util::Enable(hMenu, m_uiOn, !bCrit);
        Util::Enable(hMenu, m_uiOff, bCrit);
    }

private:
    unsigned int m_uiOn;
    unsigned int m_uiOff;
};
