// win32_baseWindow.cpp
//
// win32_utilities

#include "stdafx.h"
#include "win32_baseWindow.h"

HWND BaseWindow::StartBaseWindow
(
    HWND             const   hwndParent,
    UINT             const   uiClassStyle,
    LPCTSTR          const   szClass,
    DWORD            const   dwWindowStyle,
	PixelRect        const * pRect,
	function<bool()> const   visibilityCriterion
)
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);
    WNDCLASSEX      wcex {};

    assert(szClass != nullptr);

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style		   = uiClassStyle;
    wcex.lpfnWndProc   = BaseWndProc;
    wcex.cbClsExtra	   = 0;
    wcex.cbWndExtra	   = sizeof(void *);	
    wcex.hInstance	   = hInstance;
    wcex.hIcon		   = nullptr; 
    wcex.hCursor	   = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = szClass;
    wcex.hIconSm	   = nullptr; 

    ATOM const atom = RegisterClassEx(&wcex);

    if (atom == 0)
    {
        DWORD const dwErr = GetLastError();
        assert(dwErr == ERROR_CLASS_ALREADY_EXISTS);
    }

    HWND hwnd = CreateWindow
    (
        szClass,
        nullptr,
        dwWindowStyle,
		pRect ? pRect->GetLeft  ().GetValue() : CW_USEDEFAULT,
		pRect ? pRect->GetTop   ().GetValue() : CW_USEDEFAULT,
		pRect ? pRect->GetWidth ().GetValue() : CW_USEDEFAULT,
		pRect ? pRect->GetHeight().GetValue() : CW_USEDEFAULT,
        hwndParent,
        nullptr,
        hInstance, 
        this
   );
    assert(hwnd != nullptr);
    SetWindowHandle(hwnd);

	StartRootWindow(visibilityCriterion);

	return hwnd;
}

bool BaseWindow::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
    switch (message)
    {

    case WM_ERASEBKGND:
        return true;			// Do not erase background

    case WM_TIMER:
        OnTimer(wParam, lParam);
        return false;

    case WM_PAINT:
        OnPaint();
        return false;

    case WM_SHOWWINDOW :
        OnShow(wParam, lParam);
        return false;

    case WM_MOUSEMOVE:
        OnMouseMove(wParam, lParam);
        return false;

    case WM_LBUTTONDBLCLK:
        OnLeftButtonDblClick(wParam, lParam);
        return true;

    case WM_MOUSEWHEEL:
        OnMouseWheel(wParam, lParam);
        return true;

    case WM_MOUSELEAVE:
        if (OnMouseLeave(wParam, lParam))
            return true;
        break;

    case WM_LBUTTONDOWN:
        OnLButtonDown(wParam, lParam);
        return false;

    case WM_LBUTTONUP:
        OnLButtonUp(wParam, lParam);
        return true;

    case WM_RBUTTONDOWN:
        OnRButtonDown(wParam, lParam);
        break;

    case WM_RBUTTONUP:
        if (OnRButtonUp(wParam, lParam))
            return true;   // message completely handled, do not pass over to default processing
        break;

    case WM_SETCURSOR:
        return OnSetCursor(wParam, lParam);

    default:
        break;
    }

    return RootWindow::CommonMessageHandler(message, wParam, lParam);
}

static LRESULT CALLBACK BaseWndProc
(
    HWND   const hwnd,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
    if (message == WM_NCCREATE)    // retrieve Window instance from window creation data and associate
    {
        SetUserDataPtr(hwnd, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
        return true;
	}
    else
	{
		if (BaseWindow * pBaseWin = reinterpret_cast<BaseWindow *>(GetRootWindow(hwnd)))
        {
            if (pBaseWin->UserProc(message, wParam, lParam))
                return (LRESULT)0;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
