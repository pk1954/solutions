// WinBasics.ixx
//
// Toolbox\WinBasics

module;

#include <Windows.h>

export module WinBasics;

export import :Win32_PIXEL;

export 
{
    using HWND             = HWND;
    using HCURSOR          = HCURSOR;
    using HFONT            = HFONT;
    using HMENU            = HMENU;
    using LRESULT          = LRESULT;
    using WPARAM           = WPARAM;
    using LPARAM           = LPARAM;
    using UINT             = UINT;
    using DWORD_PTR        = DWORD_PTR;
    using HBITMAP          = HBITMAP;
    using BITMAPINFO       = BITMAPINFO;
    using BITMAPFILEHEADER = BITMAPFILEHEADER;
    using BITMAPINFOHEADER = BITMAPINFOHEADER;

    using ::IsWindowVisible;
    using ::CreateDIBSection;
}
