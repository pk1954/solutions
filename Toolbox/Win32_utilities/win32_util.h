// win32_util.h : 
//
// Toolbox\Win32_utilities

#pragma once

#include "Windows.h"
#include "NamedType.h"
#include "PixelTypes.h"
#include "BoolOp.h"

#include <string>
#include <iostream>

using std::wostream;
using std::wcout;
using std::endl;

class Script;

namespace Util
{
    wostream & operator << (wostream &, RECT const &);

    RECT ScrReadRECT(Script &);

    inline bool CtrlKeyDown() { return GetAsyncKeyState(VK_CONTROL) & 0x01; }

	union U64Bit
	{
        struct twoFloats
        {
            float floatA;
            float floatB;
        } f2;
        struct twoLongs
        {
            long longA;
            long longB;
        } l2;
        struct twoULongs
        {
            unsigned long ulA;
            unsigned long ulB;
        } ul2;
        UINT64 ui64;
	};

    inline UINT64 Pack2UINT64(MicroMeterPnt const pnt)
    {
        U64Bit u;
        u.f2.floatA = pnt.GetXvalue();
        u.f2.floatB = pnt.GetYvalue();
        return u.ui64;
    }

    inline MicroMeterPnt Unpack2MicroMeterPnt(UINT64 ui64)
	{
		U64Bit u;
		u.ui64 = ui64;
		return MicroMeterPnt(MicroMeter(u.f2.floatA), MicroMeter(u.f2.floatB));
	}

    inline UINT64 Pack2UINT64(long const lA, long const lB)
    {
        U64Bit u;
        u.l2.longA = lA;
        u.l2.longB = lB;
        return u.ui64;
    }

    inline long UnpackLongA(UINT64 ui64)
    {
        U64Bit u;
        u.ui64 = ui64;
        return u.l2.longA;
    }

    inline long UnpackLongB(UINT64 ui64)
    {
        U64Bit u;
        u.ui64 = ui64;
        return u.l2.longB;
    }

    inline UINT64 Pack2UINT64(unsigned long const ulA, unsigned long const ulB)
    {
        U64Bit u;
        u.ul2.ulA = ulA;
        u.ul2.ulB = ulB;
        return u.ui64;
    }

    inline unsigned long UnpackUlongA(UINT64 ui64)
    {
        U64Bit u;
        u.ui64 = ui64;
        return u.ul2.ulA;
    }

    inline unsigned long UnpackUlongB(UINT64 ui64)
    {
        U64Bit u;
        u.ui64 = ui64;
        return u.ul2.ulB;
    }

    inline bool operator== (RECT const & a, RECT const & b) 
	{ 
		return (a.left == b.left) && (a.top == b.top) && (a.right == b.right) && (a.bottom == b.bottom); 
	};

	inline bool operator!= (RECT const & a, RECT const & b) 
	{ 
		return (a.left != b.left) || (a.top != b.top) || (a.right != b.right) || (a.bottom != b.bottom); 
	};

	inline PixelPoint POINT2PixelPoint(POINT const & pnt) 
	{ 
		return PixelPoint{ PIXEL(pnt.x), PIXEL(pnt.y) }; 
	}

	inline POINT PixelPoint2POINT(PixelPoint const & pp) 
	{ 
		return POINT{ pp.GetXvalue(), pp.GetYvalue() }; 
	}

	inline PixelRect RECT2PixelRect(RECT const & rect) 
	{ 
		return PixelRect
		{ 
			PIXEL(rect.left), 
			PIXEL(rect.top), 
			PIXEL(rect.right), 
			PIXEL(rect.bottom) 
		}; 
	}

	inline RECT PixelRect2RECT(PixelRect const & pixRect) 
	{ 
		return RECT
		{ 
			pixRect.GetLeft  ().GetValue(),	
			pixRect.GetTop   ().GetValue(), 
			pixRect.GetRight ().GetValue(), 
			pixRect.GetBottom().GetValue() 
		}; 
	}

    inline PixelRectSize PixelRectSizeFromRECT(RECT const & rect)
    {
        return PixelRectSize
		{ 
			PIXEL(rect.right - rect.left), 
			PIXEL(rect.bottom - rect.top) 
		};
    }

	inline bool MoveWindow(HWND const hwnd, PIXEL const xPos, PIXEL const yPos, PIXEL const width, PIXEL const height, bool const bRedraw)
	{
		return ::MoveWindow(hwnd, xPos.GetValue(), yPos.GetValue(), width.GetValue(), height.GetValue(), bRedraw);
	}

	inline void SetText(HWND const hwnd, wchar_t const * const wstrText)
	{
		(void)::SendMessage(hwnd, WM_SETTEXT,	0, (LPARAM)(wstrText)	);
	}

    inline void Show(HWND const hwnd, bool const bStateOld, bool const bStateNew)
    {
        if (bStateNew != bStateOld)
            ShowWindow(hwnd, bStateNew ? SW_SHOW : SW_HIDE);
    }

    inline void Show(HWND const hwnd, bool const bStateNew)
    {
        Util::Show(hwnd, IsWindowVisible(hwnd), bStateNew);
    }

    inline bool Show(HWND const hwnd, tBoolOp const op)
    {
        bool const bStateOld = IsWindowVisible(hwnd);
        Util::Show(hwnd, ApplyOp2(bStateOld, op));
		return bStateOld;
    }

    inline RECT GetClRect(HWND const hwnd) // left / top always 0
    {
        RECT rect;
        (void)GetClientRect(hwnd, &rect);                     
        return rect;
    }

    inline PIXEL GetClientWindowHeight(HWND const hwnd)
    {
        RECT rect = GetClRect(hwnd);                    
		return PIXEL(PIXEL(rect.bottom - rect.top));
    }

    inline PIXEL GetClientWindowWidth(HWND const hwnd)
    {
        RECT rect = GetClRect(hwnd);
		return PIXEL(PIXEL(rect.right - rect.left));
    }

    inline PixelRect GetClPixelRect(HWND const hwnd) // left / top always 0
    {
		return RECT2PixelRect(GetClRect(hwnd)); 
    }

    inline PixelRectSize GetClRectSize(HWND const hwnd)
    {
        return PixelRectSizeFromRECT(GetClRect(hwnd));
    }

    inline PixelPoint GetClRectCenter(HWND const hwnd)
    {
        PixelRectSize pixSize = GetClRectSize(hwnd);
		return PixelPoint(pixSize.GetX() / 2, pixSize.GetY() / 2);
    }

    inline bool IsInClientRect(HWND const hwnd, PixelPoint const pp)  // Is point in client rect?
    {
        RECT const rect = GetClRect(hwnd);  
        return PtInRect(&rect, PixelPoint2POINT(pp));
    } 

    inline bool IsInClientRect(HWND const hwnd, PixelRect const & pixRect)  // Is rect in client rect?
    {
        RECT const rect = GetClRect(hwnd);  
		return PtInRect(&rect, PixelPoint2POINT(pixRect.GetStartPoint())) && 
			   PtInRect(&rect, PixelPoint2POINT(pixRect.GetEndPoint  ()));
    } 

	inline PixelPoint Client2Screen(HWND const hwnd, PixelPoint const & pixPoint)
    {
		POINT pnt { PixelPoint2POINT(pixPoint) };
        (void)ClientToScreen(hwnd, &pnt);
		return POINT2PixelPoint(pnt);
    }
	
    inline PixelPoint Screen2Client(HWND const hwnd, PixelPoint const & pixPoint)
    {
		POINT pnt { PixelPoint2POINT(pixPoint) };
        (void)ScreenToClient(hwnd, &pnt);
		return POINT2PixelPoint(pnt);
    }

    inline PixelPoint GetRelativeCrsrPosition(HWND const hwnd)   // Delivers cursor position relative to client area 
    {
        POINT pnt;
        (void)GetCursorPos(&pnt);
        ScreenToClient(hwnd, &pnt);
        return POINT2PixelPoint(pnt);
    }

    inline void SetRelativeCrsrPosition(HWND const hwnd, PixelPoint const & pixPoint)  
    {
        POINT pnt { PixelPoint2POINT(pixPoint) };
        ClientToScreen(hwnd, &pnt);
        SetCursorPos(pnt.x, pnt.y);
    }

    inline PixelRect GetWindowRect(HWND const hwnd)
    {
        RECT rect;
        bool bRes = GetWindowRect(hwnd, &rect);
        assert(bRes);
        return RECT2PixelRect(rect);
    }

    inline PixelRectSize GetWindowSize(HWND const hwnd)
    {
        RECT rect;
        bool bRes = GetWindowRect(hwnd, &rect);
        assert(bRes);
        return PixelRectSizeFromRECT(rect);
    }

    inline PIXEL GetWindowWidth(HWND const hwnd)
    {
		return GetWindowSize(hwnd).GetX();
    }

    inline PIXEL GetWindowHeight(HWND const hwnd)
    {
		return GetWindowSize(hwnd).GetY();
    }

    inline PIXEL GetWindowBottom(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
		return PIXEL(PIXEL(rect.bottom));
    }

    inline PIXEL GetWindowTop(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
		return PIXEL(PIXEL(rect.top));
    }

    inline PIXEL GetWindowLeftPos(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
		return PIXEL(PIXEL(rect.left));
    }

    inline PIXEL GetWindowRightPos(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
		return PIXEL(PIXEL(rect.right));
    }

    inline bool CrsrInClientRect(HWND const hwnd)  // Is cursor position in client rect?
    {
		return IsInClientRect(hwnd, GetRelativeCrsrPosition(hwnd) );
    } 

    inline void FastFill(HDC const hDC, RECT const & rect)
    {
        (void)ExtTextOut(hDC, 0, 0, ETO_OPAQUE, & rect, L"", 0, 0);
    }

    inline void FastFill(HDC const hDC, PixelRect const & pixRect)
    {
        FastFill(hDC, PixelRect2RECT(pixRect));
    }

    inline void FastFill(HDC const hDC, HWND const hwnd)
    {
        Util::FastFill(hDC, GetClRect(hwnd));
    }

    inline void AddWindowStyle(HWND const hwnd, DWORD const dwStyle)
    {
        DWORD const dwOldStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        DWORD const dwNewStyle = dwOldStyle | dwStyle;
        SetWindowLong(hwnd, GWL_EXSTYLE, dwNewStyle);
    }

    inline void DeleteWindowStyle(HWND const hwnd, DWORD const dwStyle)
    {
        DWORD const dwOldStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        DWORD const dwNewStyle = dwOldStyle & ~dwStyle;
        LONG  const lRes       = SetWindowLong(hwnd, GWL_EXSTYLE, dwNewStyle);
        assert(lRes == dwNewStyle);
    }

    inline MONITORINFO GetMonitorInfo(HMONITOR hMonitor)
    {
        MONITORINFO monInfo;
        monInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &monInfo);
        return monInfo;
    }

	void MakeLayered(HWND const , bool const, COLORREF const, UINT const);

    void AdjustRight(HWND const, PIXEL const);
    void AdjustLeft (HWND const, PIXEL const);

    PixelRect CalcWindowRect(PixelRect const, DWORD const);

    bool MoveWindowAbsolute(HWND const, PixelRect  const &, bool const);
    bool MoveWindowAbsolute(HWND const, PixelPoint const &, bool const);
	
    DWORD     GetNrOfCPUs();
    ULONGLONG GetPhysicalMemory();
    wstring   GetCurrentDateAndTime();
    wstring   GetComputerName();
    wstring   GetUserName();
    wstring   GetCurrentDir();
    void      SetApplicationTitle(HWND const, int const, wstring const & = L"");
    void      StdOutConsole();
    bool      EscapeKeyPressed();
    HMENU     PopupMenu(HMENU const, LPCTSTR const);
    void      SetNotifyByPos(HMENU const);
    void      SetMenuItemData(HMENU const, UINT const, UINT_PTR const);
    UINT_PTR  GetMenuItemData(HMENU const, UINT const);
    void      AddMenu        (HMENU const, UINT const, UINT_PTR const, LPCWSTR const);
    void      InsertMenuItem (HMENU const, UINT const, UINT_PTR const, wstring const &);
};
