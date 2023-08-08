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
        unsigned int const uiCmdId
    )
      : m_uiCmdId(uiCmdId)
    { }

    void AppendOnOffMenu(HMENU hMenu, LPCTSTR const title) const
    {
        ::AddMenu(hMenu, MF_STRING, m_uiCmdId, title);
    }

    void EnableOnOff(HMENU hMenu, bool const bCrit)
    {
        ::CheckMenuItem(hMenu, m_uiCmdId, bCrit);
    }

private:
    unsigned int m_uiCmdId;
};
