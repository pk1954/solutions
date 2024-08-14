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
    using HINSTANCE        = HINSTANCE;
    using LRESULT          = LRESULT;
    using WPARAM           = WPARAM;
    using LPARAM           = LPARAM;
    using UINT             = UINT;
    using DWORD            = DWORD;
    using WORD             = WORD;
    using DWORD_PTR        = DWORD_PTR;
    using LPCWSTR          = LPCWSTR;
    using LPCTSTR          = LPCTSTR;
    using HBITMAP          = HBITMAP;
    using BITMAPINFO       = BITMAPINFO;
    using BITMAPFILEHEADER = BITMAPFILEHEADER;
    using BITMAPINFOHEADER = BITMAPINFOHEADER;
    using PAINTSTRUCT      = PAINTSTRUCT;

    using ::AppendMenuW;
    using ::BringWindowToTop;
    using ::CreateDIBSection;
    using ::CreateMenu;
    using ::DrawMenuBar;
    using ::EnableWindow;
    using ::GetModuleHandleW;
    using ::IsWindowVisible;
    using ::LoadCursorW;
    using ::MessageBoxW;
    using ::ReleaseCapture;
    using ::SetCursor;
    using ::SetMenu;
    using ::SetWindowPos;
    using ::SetWindowTextW;

    constexpr WORD LoWord(DWORD value) { return static_cast<WORD>(value & 0xFFFF); }

    constexpr int MouseWheelDelta(WPARAM const wParam) 
    { 
        return GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; 
    };
}
