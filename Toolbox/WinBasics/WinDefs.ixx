// WinDefs.ixx
//
// Toolbox\WinDefs

module;

#include <Windows.h>

export module WinBasics:WinDefs;

export 
{
    using ACCEL                 = ACCEL;
    using HWND                  = HWND;
    using HCURSOR               = HCURSOR;
    using HFONT                 = HFONT;
    using HMENU                 = HMENU;
    using HGDIOBJ               = HGDIOBJ;
    using HACCEL                = HACCEL;
    using HINSTANCE             = HINSTANCE;
    using LRESULT               = LRESULT;
    using WPARAM                = WPARAM;
    using LPARAM                = LPARAM;
    using UINT                  = UINT;
    using DWORD                 = DWORD;
    using WORD                  = WORD;
    using DWORD_PTR             = DWORD_PTR;
    using LPACCEL               = LPACCEL;
    using LPCWSTR               = LPCWSTR;
    using LPCTSTR               = LPCTSTR;
    using LPPOINT               = LPPOINT;
    using SRWLOCK               = SRWLOCK;
    using HBITMAP               = HBITMAP;
    using BITMAPINFO            = BITMAPINFO;
    using BITMAPFILEHEADER      = BITMAPFILEHEADER;
    using BITMAPINFOHEADER      = BITMAPINFOHEADER;
    using PAINTSTRUCT           = PAINTSTRUCT;
    using DRAWITEMSTRUCT        = DRAWITEMSTRUCT;
    using TRACKMOUSEEVENT       = TRACKMOUSEEVENT;
    using PTP_CALLBACK_INSTANCE = PTP_CALLBACK_INSTANCE;

    using ::AcquireSRWLockExclusive;
    using ::AcquireSRWLockShared;
    using ::AppendMenuW;
    using ::BringWindowToTop;
    using ::CreateAcceleratorTableW;
    using ::CreateDIBSection;
    using ::CreateMenu;
    using ::DrawMenuBar;
    using ::EnableWindow;
    using ::EndDialog;
    using ::GetModuleHandleW;
    using ::GetStockObject;
    using ::InitializeSRWLock;
    using ::IsWindowVisible;
    using ::LoadCursorW;
    using ::MapWindowPoints;
    using ::MessageBoxW;
    using ::PostQuitMessage;
    using ::ReleaseCapture;
    using ::ReleaseSRWLockExclusive;
    using ::ReleaseSRWLockShared;
    using ::SetBkColor;
    using ::SetCursor;
    using ::SetMenu;
    using ::SetWindowPos;
    using ::SetWindowTextW;
    using ::ShellExecuteW;
    using ::TryAcquireSRWLockExclusive;

    constexpr WORD LoWord(DWORD_PTR value) { return static_cast<WORD>(value & 0xFFFF); }

    constexpr long MakeLong(auto a, auto b) { return MAKELONG(a, b); }

    constexpr int MouseWheelDelta(WPARAM const wParam) 
    { 
        return GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; 
    };
}
