// Win32_DialogTemplate.ixx
//
// Win32_utilities

module;

#include <Windows.h>

export module DialogTemplate;

struct DlgTemplate
{
    DWORD style;
    DWORD dwExtendedStyle;
    WORD  ccontrols;
    short x;
    short y;
    short cx;
    short cy;
    WORD  menu;              // name or ordinal of a menu resource
    WORD  windowClass;       // name or ordinal of a window class
    WCHAR wszTitle[1];       // title string of the dialog box
};

export inline const DlgTemplate EMPTY_TEMPLATE
{
   WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME,
   0x0,                     // exStyle;
   0,                       // ccontrols
   0, 0, 0, 0,
   0,                       // menu: none
   0,                       // window class: none
   L""                      // Window caption
};
