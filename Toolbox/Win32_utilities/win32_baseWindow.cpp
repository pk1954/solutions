// Win32_BaseWindow.cpp
//
// Win32_utilities

module;

#include <string>
#include <cassert>
#include <chrono>
#include "Windows.h"

module BaseWindow;

import Win32_Util_Resource;
import Util;
import Types;
import Win32_Util;
import RootWindow;

using std::wstring;
using std::bit_cast;
using std::chrono::milliseconds;
using std::wostringstream;

HWND BaseWindow::StartBaseWindow
(
    HWND      const   hwndParent,
    UINT      const   uiClassStyle,
    LPCTSTR   const   szClass,
    DWORD     const   dwWindowStyle,
	PixelRect const * pRect,
	VisCrit   const & visibilityCriterion
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
    SetWindowText(GetCaption());

	return hwnd;
}

void BaseWindow::trackMouse(bool const bOn) const
{
    TRACKMOUSEEVENT tme;
    tme.cbSize      = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags     = TME_HOVER | TME_LEAVE;
    tme.hwndTrack   = bOn ? GetWindowHandle() : HWND(0);
    tme.dwHoverTime = HOVER_DEFAULT;
    TrackMouseEvent(&tme);
};

void BaseWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
    trackMouse(true);
};

void BaseWindow::OnMouseLeave()
{
    trackMouse(false);
};

wstring BaseWindow::GetCaption() const 
{ 
    wstring wstrCaption { L"+++++" };

    if (WindowHasCaption())
    {
        wstrCaption = GetWindowText();
    }
    else if (BaseWindow const * pBaseWinParent{ static_cast<BaseWindow const *>(GetParentRootWindow()) } )
    {
        wstrCaption = pBaseWinParent->GetCaption();
    }
    return wstrCaption;
}

void BaseWindow::SetCaption() const
{
    wstring const caption { m_bPerfMonMode ? Format2wstring(m_usPaintTime, 1) : GetCaption() };
    if (WindowHasCaption())
    {
        SetWindowText(caption);
    }
    else if (BaseWindow const * pBaseWinParent{ static_cast<BaseWindow const *>(GetParentRootWindow()) } )
    {
        pBaseWinParent->SetWindowText(caption);
    }
}

void BaseWindow::OnMouseHover(WPARAM const wParam, LPARAM const lParam)
{
 //   SetCaption();
};

bool BaseWindow::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
    switch (message)
    {

    case WM_ERASEBKGND:
        return true;			// Do not erase background

    case WM_NOTIFY:
        OnNotify(wParam, lParam);
        return false;

    case WM_TIMER:
        OnTimer(wParam, lParam);
        return false;

    case WM_CHAR:
        OnChar(wParam, lParam);
        return false;

    case WM_MOUSEHOVER:
        OnMouseHover(wParam, lParam);
        return false;

    case WM_PAINT:
        if (m_bPerfMonMode)
        {
            m_paintTimer.Start();
            OnPaint();
            m_paintTimer.Stop();
            m_usPaintTime = m_paintTimer.GetMicroSecsTilStart();
        }
        else
            OnPaint();
        return false;

    case WM_SHOWWINDOW :
        OnShow(wParam, lParam);
        return false;

    case WM_MOUSEMOVE:
        OnMouseMove(wParam, lParam);
        return false;

    case WM_LBUTTONDBLCLK:
        OnLButtonDblClick(wParam, lParam);
        return true;

    case WM_NCLBUTTONDBLCLK:
        OnNCLButtonDblClick(wParam, lParam);
        return true;

    case WM_MOUSEWHEEL:
        OnMouseWheel(wParam, lParam);
        return true;

    case WM_MOUSELEAVE:
        OnMouseLeave();
        return false;

    case WM_LBUTTONDOWN:
        return OnLButtonDown(wParam, lParam);

    case WM_LBUTTONUP:
        return OnLButtonUp(wParam, lParam);

    case WM_RBUTTONDOWN:
        OnRButtonDown(wParam, lParam);
        break;

    case WM_RBUTTONUP:
        if (OnRButtonUp(wParam, lParam))
            return true;   // message completely handled, do not pass over to default processing
        break;

    case WM_SETCURSOR:
        return OnSetCursor(wParam, lParam);

    case WM_DRAWITEM:
        OnDrawItem(wParam, bit_cast<DRAWITEMSTRUCT const *>(lParam));
        return true;

    case WM_APP_CAPTION:
        SetCaption();
        break;

    case WM_APP_SCALE_COMMAND:
        OnScaleCommand(wParam, lParam);
        break;

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
        auto pCreateStruct { bit_cast<LPCREATESTRUCT>(lParam) };
        Util::SetUserDataPtr(hwnd, bit_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        return true;
	}
    else
	{
		if (auto * pBaseWin = static_cast<BaseWindow *>(GetRootWindow(hwnd)))
        {
            if (pBaseWin->UserProc(message, wParam, lParam))
                return (LRESULT)0;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
