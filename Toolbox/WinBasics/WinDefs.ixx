// WinDefs.ixx
//
// Toolbox\WinDefs

module;

#include <Windows.h>

export module WinBasics:WinDefs;

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
    using DWORD            = DWORD;
    using WORD             = WORD;
    using DWORD_PTR        = DWORD_PTR;
    using LPCWSTR          = LPCWSTR;
    using HBITMAP          = HBITMAP;
    using BITMAPINFO       = BITMAPINFO;
    using BITMAPFILEHEADER = BITMAPFILEHEADER;
    using BITMAPINFOHEADER = BITMAPINFOHEADER;
    using PAINTSTRUCT      = PAINTSTRUCT;

    using ::AppendMenuW;
    using ::CreateDIBSection;
    using ::EnableWindow;
    using ::IsWindowVisible;
    using ::LoadCursorW;
    using ::MessageBoxW;
    using ::ReleaseCapture;
    using ::SetCursor;

    constexpr WORD LoWord(DWORD value) { return static_cast<WORD>(value & 0xFFFF); }

    constexpr int MouseWheelDelta(WPARAM const wParam) 
    { 
        return GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; 
    };
}
