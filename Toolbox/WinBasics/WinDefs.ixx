// WinDefs.ixx
//
// Toolbox\WinDefs

module;

#include <Windows.h>

export module WinBasics:WinDefs;

import std;

using std::wstring;

export 
{
    using ATOM                  = ATOM;
    using MSG                   = MSG;
    using ACCEL                 = ACCEL;
    using HWND                  = HWND;
    using HDC                   = HDC;
    using HCURSOR               = HCURSOR;
    using HFONT                 = HFONT;
    using HMENU                 = HMENU;
    using HMONITOR              = HMONITOR;
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
    using INT_PTR               = INT_PTR;
    using LPACCEL               = LPACCEL;
    using COLORREF              = COLORREF;
    using LPCWSTR               = LPCWSTR;
    using LPCSTR                = LPCSTR;
    using LPWSTR                = LPWSTR;
    using LPCTSTR               = LPCTSTR;
    using LPPAINTSTRUCT         = LPPAINTSTRUCT;
    using LPCREATESTRUCT        = LPCREATESTRUCT;
    using LPPOINT               = LPPOINT;
    using LPRECT                = LPRECT;
    using LONG_PTR              = LONG_PTR;
    using SRWLOCK               = SRWLOCK;
    using HBITMAP               = HBITMAP;
    using BITMAPINFO            = BITMAPINFO;
    using BITMAPFILEHEADER      = BITMAPFILEHEADER;
    using BITMAPINFOHEADER      = BITMAPINFOHEADER;
    using PAINTSTRUCT           = PAINTSTRUCT;
    using DRAWITEMSTRUCT        = DRAWITEMSTRUCT;
    using TRACKMOUSEEVENT       = TRACKMOUSEEVENT;
    using PTP_CALLBACK_INSTANCE = PTP_CALLBACK_INSTANCE;
    using WNDCLASSEX            = WNDCLASSEX;
    using MONITORINFO           = MONITORINFO;

    using ::AcquireSRWLockExclusive;
    using ::AcquireSRWLockShared;
    using ::AppendMenuW;
    using ::BeginPaint;
    using ::BringWindowToTop;
    using ::CheckRadioButton;
    using ::CreateAcceleratorTableW;
    using ::CreateCompatibleBitmap;
    using ::CreateDIBSection;
    using ::CreateFontW;
    using ::CreateMenu;
    using ::CreatePopupMenu;
    using ::DefWindowProcW;
    using ::GetClientRect;
    using ::GetMessageW;
    using ::GetModuleHandleW;
    using ::CreateWindowExW;
    using ::DestroyMenu;
    using ::DestroyWindow;
    using ::DispatchMessageW;
    using ::DrawMenuBar;
    using ::EnableMenuItem;
    using ::EnableWindow;
    using ::EndDialog;
    using ::EndPaint;
    using ::EnumDisplayMonitors;
    using ::GetCapture;
    using ::GetDlgItem;
    using ::GetDlgCtrlID;
    using ::GetLastError;
    using ::GetParent;
    using ::GetMonitorInfoW;
    using ::GetStockObject;
    using ::GetSystemMetrics;
    using ::GetWindowLong;
    using ::GetWindowTextW;
    using ::GetWindowTextLengthW;
    using ::InitializeSRWLock;
    using ::InvalidateRect;
    using ::IsChild;
    using ::IsDlgButtonChecked;
    using ::IsDialogMessageW;
    using ::IsWindowVisible;
    using ::IsZoomed;
    using ::LoadCursorW;
    using ::MapWindowPoints;
    using ::MessageBoxW;
    using ::MoveWindow;
    using ::MulDiv;
    using ::MultiByteToWideChar;
    using ::PeekMessageW;
    using ::PostMessageW;
    using ::PostQuitMessage;
    using ::RegisterClassEx;
    using ::ReleaseCapture;
    using ::ReleaseSRWLockExclusive;
    using ::ReleaseSRWLockShared;
    using ::SendDlgItemMessageW;
    using ::SendMessageW;
    using ::SendNotifyMessageW;
    using ::SetBkColor;
    using ::SetCapture;
    using ::SetConsoleOutputCP;
    using ::SetCursor;
    using ::SetFocus;
    using ::SetForegroundWindow;
    using ::SetLastError;
    using ::SetMenu;
    using ::SetTextAlign;
    using ::SetWindowPos;
    using ::SetWindowTextW;
    using ::ShellExecuteW;
    using ::ShowWindow;
    using ::TextOutW;
    using ::TrackMouseEvent;
    using ::TrackPopupMenu;
    using ::TranslateAcceleratorW;
    using ::TranslateMessage;
    using ::TryAcquireSRWLockExclusive;
    using ::UpdateWindow;

    constexpr WORD LoWord(DWORD_PTR const lp) { return static_cast<WORD>( lp        & 0xffff); }
    constexpr WORD HiWord(DWORD_PTR const lp) { return static_cast<WORD>((lp >> 16) & 0xffff); }

    constexpr WORD GetXlparam(LPARAM const lp) { return LoWord(lp); }
    constexpr WORD GetYlparam(LPARAM const lp) { return HiWord(lp); }

    constexpr long MakeLong(auto a, auto b) { return MAKELONG(a, b); }

    constexpr int MouseWheelDelta(WPARAM const wParam) 
    { 
        return GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; 
    };

    constexpr COLORREF MakeRGB(WORD const r, WORD const g, WORD const b)
    {
        return RGB(r, g, b);
    }
}
