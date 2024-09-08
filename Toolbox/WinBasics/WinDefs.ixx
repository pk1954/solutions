// WinDefs.ixx
//
// Toolbox\WinDefs

module;

#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include <Windows.h>
#include <CommCtrl.h>

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

    using ::_setmode;
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
    using ::GetMessageW;
    using ::GetModuleHandleW;
    using ::CreateWindowExW;
    using ::DefSubclassProc;
    using ::DestroyMenu;
    using ::DestroyWindow;
    using ::DispatchMessageW;
    using ::DrawMenuBar;
    using ::EnableMenuItem;
    using ::EnableWindow;
    using ::EndDialog;
    using ::EndPaint;
    using ::GetCapture;
    using ::GetDlgItem;
    using ::GetDlgCtrlID;
    using ::GetLastError;
    using ::GetParent;
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
    using ::LoadCursorW;
    using ::MapWindowPoints;
    using ::MessageBoxW;
    using ::MoveWindow;
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
    using ::SetWindowPos;
    using ::SetWindowSubclass;
    using ::SetWindowTextW;
    using ::ShellExecuteW;
    using ::ShowWindow;
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

    bool SwitchWcoutTo(wstring const & wszTraceFileName)
    { 
        FILE  * fp;
        errno_t res = _wfreopen_s(&fp, wszTraceFileName.c_str(), L"w", stdout);
        _setmode(_fileno(stdout), _O_U8TEXT);  // set code page to UTF-8
        SetConsoleOutputCP(CP_UTF8);           // for printing Unicode
        return res == 0;
    }
}
