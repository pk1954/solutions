// OnOffPair.ixx
//
// Win32_utilities

export module OnOffPair;

import Win32_Util;
import WinBasics;

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

    unsigned int CmdId() const { return m_uiCmdId; }

private:
    unsigned int m_uiCmdId;
};
