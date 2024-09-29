// WinDefs.ixx
//
// Toolbox\WinDefs

module;

#include <Windows.h>
#include <Windowsx.h>

export module WinBasics:WinDefs;

import std;

using std::wstring;

export 
{
    using ACCEL                 = ACCEL;
    using ATOM                  = ATOM;
    using BITMAPINFO            = BITMAPINFO;
    using BITMAPFILEHEADER      = BITMAPFILEHEADER;
    using BITMAPINFOHEADER      = BITMAPINFOHEADER;
    using CHOOSECOLOR           = CHOOSECOLOR;
    using DRAWITEMSTRUCT        = DRAWITEMSTRUCT;
    using DWORD                 = DWORD;
    using DWORD_PTR             = DWORD_PTR;
    using FILETIME              = FILETIME;
    using HACCEL                = HACCEL;
    using HBITMAP               = HBITMAP;
    using HDC                   = HDC;
    using HCURSOR               = HCURSOR;
    using HFONT                 = HFONT;
    using HMENU                 = HMENU;
    using HINSTANCE             = HINSTANCE;
    using HMONITOR              = HMONITOR;
    using HGDIOBJ               = HGDIOBJ;
    using HWND                  = HWND;
    using INT_PTR               = INT_PTR;
    using LRESULT               = LRESULT;
    using LPARAM                = LPARAM;
    using LPACCEL               = LPACCEL;
    using COLORREF              = COLORREF;
    using LPCWSTR               = LPCWSTR;
    using LPCSTR                = LPCSTR;
    using LPWSTR                = LPWSTR;
    using LPCTSTR               = LPCTSTR;
    using LPDWORD               = LPDWORD;
    using LPPAINTSTRUCT         = LPPAINTSTRUCT;
    using LPCREATESTRUCT        = LPCREATESTRUCT;
    using LPPOINT               = LPPOINT;
    using LPRECT                = LPRECT;
    using LONG_PTR              = LONG_PTR;
    using MONITORINFO           = MONITORINFO;
    using MSG                   = MSG;
    using PAINTSTRUCT           = PAINTSTRUCT;
    using PTP_CALLBACK_INSTANCE = PTP_CALLBACK_INSTANCE;
    using PTP_TIMER             = PTP_TIMER;
    using PVOID                 = PVOID;
    using SRWLOCK               = SRWLOCK;
    using TRACKMOUSEEVENT       = TRACKMOUSEEVENT;
    using UINT                  = UINT;
    using WNDCLASSEX            = WNDCLASSEX;
    using WORD                  = WORD;
    using WPARAM                = WPARAM;

    using ::AcquireSRWLockExclusive;
    using ::AcquireSRWLockShared;
    using ::AppendMenuW;
    using ::BeginPaint;
    using ::BitBlt;
    using ::BringWindowToTop;
    using ::CheckRadioButton;
    using ::ChooseColorW;
    using ::CloseThreadpoolTimer;
    using ::CreateAcceleratorTableW;
    using ::CreateCompatibleBitmap;
    using ::CreateCompatibleDC;
    using ::CreateDIBSection;
    using ::CreateFontW;
    using ::CreateMenu;
    using ::CreatePopupMenu;
    using ::CreateThreadpoolTimer;
    using ::CreateWindowExW;
    using ::DefWindowProcW;
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
    using ::GetClientRect;
    using ::GetDC;
    using ::GetDlgItem;
    using ::GetDlgCtrlID;
    using ::GetMessageW;
    using ::GetModuleHandleW;
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
    using ::IsThreadpoolTimerSet;
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
    using ::ReleaseDC;
    using ::ReleaseSRWLockExclusive;
    using ::ReleaseSRWLockShared;
    using ::SelectObject;
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
    using ::SetThreadpoolTimer;
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
    using ::WaitForThreadpoolTimerCallbacks;
    using ::UpdateWindow;

    WORD LoWord(DWORD_PTR const lp) { return static_cast<WORD>( lp        & 0xffff); }
    WORD HiWord(DWORD_PTR const lp) { return static_cast<WORD>((lp >> 16) & 0xffff); }

    long CrsrXpos(LPARAM const lParam) { return GET_X_LPARAM(lParam); }
	long CrsrYpos(LPARAM const lParam) { return GET_Y_LPARAM(lParam); }

    long MakeLong(auto a, auto b) { return MAKELONG(a, b); }

    int MouseWheelDelta(WPARAM const wParam) 
    { 
        return GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; 
    };

    constexpr COLORREF MakeRGB(WORD const r, WORD const g, WORD const b)
    {
        return RGB(r, g, b);
    }
}
