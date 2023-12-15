// DialogItem.ixx
//
// Toolbox\Win32_utilities

module;

#include <string>
#include <Windows.h>

export module DialogItem;

using std::wstring;

export class DialogItem
{
public:
    DialogItem(wstring const &wstrText, LONG_PTR const id)
        : m_wstrText(wstrText),
          m_id(id)
    {}

void SetState
(
    HWND const hwnd, 
    bool const bEnabled
)
{
//    ::SetDlgItemState(hwnd, m_wstrText, m_id, bEnabled);
}

private:
    wstring  m_wstrText;
    LONG_PTR m_id;
};