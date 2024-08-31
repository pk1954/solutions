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
    using MSG                   = MSG;
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
    using INT_PTR               = INT_PTR;
    using LPACCEL               = LPACCEL;
    using COLORREF              = COLORREF;
    using LPCWSTR               = LPCWSTR;
    using LPCSTR                = LPCSTR;
    using LPWSTR                = LPWSTR;
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

    using ::_setmode;
    using ::AcquireSRWLockExclusive;
    using ::AcquireSRWLockShared;
    using ::AppendMenuW;
    using ::BringWindowToTop;
    using ::CreateAcceleratorTableW;
    using ::CreateDIBSection;
    using ::CreateMenu;
    using ::GetMessageW;
    using ::GetModuleHandleW;
    using ::CreateWindowExW;
    using ::DefSubclassProc;
    using ::DispatchMessageW;
    using ::DrawMenuBar;
    using ::EnableWindow;
    using ::EndDialog;
    using ::GetDlgCtrlID;
    using ::GetLastError;
    using ::GetModuleHandleW;
    using ::GetStockObject;
    using ::GetSystemMetrics;
    using ::InitializeSRWLock;
    using ::IsChild;
    using ::IsDialogMessageW;
    using ::IsWindowVisible;
    using ::LoadCursorW;
    using ::MapWindowPoints;
    using ::MessageBoxW;
    using ::MultiByteToWideChar;
    using ::PeekMessageW;
    using ::PostQuitMessage;
    using ::ReleaseCapture;
    using ::ReleaseSRWLockExclusive;
    using ::ReleaseSRWLockShared;
    using ::SetBkColor;
    using ::SetConsoleOutputCP;
    using ::SetCursor;
    using ::SetLastError;
    using ::SetMenu;
    using ::SetWindowPos;
    using ::SetWindowSubclass;
    using ::SetWindowTextW;
    using ::ShellExecuteW;
    using ::TranslateAcceleratorW;
    using ::TranslateMessage;
    using ::TryAcquireSRWLockExclusive;

    constexpr WORD LoWord(DWORD_PTR value) { return static_cast<WORD>(value & 0xFFFF); }

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
