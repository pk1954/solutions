// Win32_PIXEL.ixx
//
// Toolbox\WinBasics

module;

#include <cassert>
#include <iostream>
#include "Windows.h"

export module Win32_PIXEL;

import Types;
import Script;

using std::wostream;

namespace Util
{
    export wostream & operator << (wostream &, RECT const &);

    export RECT ScrReadRECT(Script &);

    export inline bool CtrlKeyDown() { return GetAsyncKeyState(VK_CONTROL) & 0x01; }

    export inline bool operator== (RECT const & a, RECT const & b) 
    { 
        return (a.left == b.left) && (a.top == b.top) && (a.right == b.right) && (a.bottom == b.bottom); 
    };

    export inline bool operator!= (RECT const & a, RECT const & b) 
    { 
        return (a.left != b.left) || (a.top != b.top) || (a.right != b.right) || (a.bottom != b.bottom); 
    };

    export inline PixelPoint POINT2PixelPoint(POINT const & pnt) 
    { 
        return PixelPoint{ PIXEL(pnt.x), PIXEL(pnt.y) }; 
    }

    export inline POINT PixelPoint2POINT(PixelPoint const & pp) 
    { 
        return POINT{ pp.GetXvalue(), pp.GetYvalue() }; 
    }

    export inline PixelRect RECT2PixelRect(RECT const & rect) 
    { 
        return PixelRect
        { 
            PIXEL(rect.left), 
            PIXEL(rect.top), 
            PIXEL(rect.right), 
            PIXEL(rect.bottom) 
        }; 
    }

    export inline RECT PixelRect2RECT(PixelRect const & pixRect) 
    { 
        return RECT
        { 
            pixRect.GetLeft  ().GetValue(),	
            pixRect.GetTop   ().GetValue(), 
            pixRect.GetRight ().GetValue(), 
            pixRect.GetBottom().GetValue() 
        }; 
    }

    export inline PixelRectSize PixelRectSizeFromRECT(RECT const & rect)
    {
        return PixelRectSize
        { 
            PIXEL(rect.right  - rect.left + 1), 
            PIXEL(rect.bottom - rect.top  + 1)
        };
    }

    export inline bool MoveWindow(HWND const hwnd, PIXEL const xPos, PIXEL const yPos, PIXEL const width, PIXEL const height, bool const bRedraw)
    {
        return ::MoveWindow(hwnd, xPos.GetValue(), yPos.GetValue(), width.GetValue(), height.GetValue(), bRedraw);
    }

    export inline bool MoveWindow(HWND const hwnd, PixelRect const rect, bool const bRedraw)
    {
        return MoveWindow(hwnd, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), bRedraw);
    }

    export inline bool MoveWindow(HWND const hwnd, PIXEL const xPos, PIXEL const yPos)
    {
        return ::SetWindowPos(hwnd, HWND_TOP, xPos.GetValue(), yPos.GetValue(), 0, 0, SWP_NOSIZE);
    }

    export inline bool MoveWindow(HWND const hwnd, PixelPoint const pos)
    {
        return MoveWindow(hwnd, pos.GetX(), pos.GetY());
    }

    export inline RECT GetClRect(HWND const hwnd) // left / top always 0
    {
        RECT rect;
        (void)GetClientRect(hwnd, &rect);                     
        return rect;
    }

    export inline PIXEL GetClientWindowHeight(HWND const hwnd)
    {
        RECT rect { GetClRect(hwnd) };
        return PIXEL(PIXEL(rect.bottom - rect.top + 1));
    }

    export inline PIXEL GetClientWindowWidth(HWND const hwnd)
    {
        RECT rect { GetClRect(hwnd) };
        return PIXEL(PIXEL(rect.right - rect.left + 1));
    }

    export inline PixelRect GetClPixelRect(HWND const hwnd) // left / top always 0
    {
        return RECT2PixelRect(GetClRect(hwnd)); 
    }

    export inline PixelRectSize GetClRectSize(HWND const hwnd)
    {
        return PixelRectSizeFromRECT(GetClRect(hwnd));
    }

    export inline PixelPoint GetClRectCenter(HWND const hwnd)
    {
        PixelRectSize pixSize { GetClRectSize(hwnd) };
        return PixelPoint(pixSize.GetX() / 2, pixSize.GetY() / 2);
    }

    export inline bool IsInClientRect(HWND const hwnd, PixelPoint const pp)  // Is point in client rect?
    {
        RECT const rect { GetClRect(hwnd) };
        return PtInRect(&rect, PixelPoint2POINT(pp));
    } 

    export inline bool IsInClientRect(HWND const hwnd, PixelRect const & pixRect)  // Is rect in client rect?
    {
        RECT const rect { GetClRect(hwnd) };
        return PtInRect(&rect, PixelPoint2POINT(pixRect.GetStartPoint())) && 
            PtInRect(&rect, PixelPoint2POINT(pixRect.GetEndPoint  ()));
    } 

    export inline PixelPoint Client2Screen(HWND const hwnd, PixelPoint const & pixPoint)
    {
        POINT pnt { PixelPoint2POINT(pixPoint) };
        (void)ClientToScreen(hwnd, &pnt);
        return POINT2PixelPoint(pnt);
    }

    export inline PixelPoint Screen2Client(HWND const hwnd, PixelPoint const & pixPoint)
    {
        POINT pnt { PixelPoint2POINT(pixPoint) };
        (void)ScreenToClient(hwnd, &pnt);
        return POINT2PixelPoint(pnt);
    }

    export inline PixelPoint GetRelativeCrsrPosition(HWND const hwnd)   // Delivers cursor position relative to client area 
    {
        POINT pnt;
        (void)GetCursorPos(&pnt);
        ScreenToClient(hwnd, &pnt);
        return POINT2PixelPoint(pnt);
    }

    export inline void SetRelativeCrsrPosition(HWND const hwnd, PixelPoint const & pixPoint)  
    {
        POINT pnt { PixelPoint2POINT(pixPoint) };
        ClientToScreen(hwnd, &pnt);
        SetCursorPos(pnt.x, pnt.y);
    }

    export inline PixelRect GetWindowRect(HWND const hwnd)
    {
        RECT rect;
        BOOL bRes { GetWindowRect(hwnd, &rect) };
        assert(bRes);
        return RECT2PixelRect(rect);
    }

    export inline PixelRectSize GetWindowSize(HWND const hwnd)
    {
        RECT rect;
        BOOL bRes { GetWindowRect(hwnd, &rect) };
        assert(bRes);
        return PixelRectSizeFromRECT(rect);
    }

    export inline PIXEL GetWindowWidth(HWND const hwnd)
    {
        return GetWindowSize(hwnd).GetX();
    }

    export inline PIXEL GetWindowHeight(HWND const hwnd)
    {
        return GetWindowSize(hwnd).GetY();
    }

    export inline PIXEL GetWindowBottom(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
        return PIXEL(PIXEL(rect.bottom));
    }

    export inline PIXEL GetWindowTop(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
        return PIXEL(PIXEL(rect.top));
    }

    export inline PIXEL GetWindowLeftPos(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
        return PIXEL(PIXEL(rect.left));
    }

    export inline PIXEL GetWindowRightPos(HWND const hwnd)
    {
        RECT rect;
        (void)GetWindowRect(hwnd, &rect);
        return PIXEL(PIXEL(rect.right));
    }

    export inline void SetWindowHeight(HWND const hwnd, PIXEL const newHeight, bool const bRedraw)
    {
        PixelRect rect { GetWindowRect(hwnd) };
        rect.SetHeight(newHeight);
        MoveWindow(hwnd, rect, bRedraw);
    }

    export inline void SetWindowWidth(HWND const hwnd, PIXEL const newWidth, bool const bRedraw)
    {
        PixelRect rect{ GetWindowRect(hwnd) };
        rect.SetWidth(newWidth);
        MoveWindow(hwnd, rect, bRedraw);
    }

    export inline void SetWindowSize(HWND const hwnd, PIXEL const newWidth, PIXEL const newHeight, bool const bRedraw)
    {
        PixelRect rect{ GetWindowRect(hwnd) };
        rect.SetWidth (newWidth);
        rect.SetHeight(newHeight);
        MoveWindow(hwnd, rect, bRedraw);
    }

    export inline bool CrsrInClientRect(HWND const hwnd)  // Is cursor position in client rect?
    {
        return IsInClientRect(hwnd, GetRelativeCrsrPosition(hwnd) );
    } 

    export inline void FastFill(HDC const hDC, RECT const & rect)
    {
        (void)ExtTextOut(hDC, 0, 0, ETO_OPAQUE, & rect, L"", 0, 0);
    }

    export inline void FastFill(HDC const hDC, PixelRect const & pixRect)
    {
        FastFill(hDC, PixelRect2RECT(pixRect));
    }

    export inline void FastFill(HDC const hDC, HWND const hwnd)
    {
        Util::FastFill(hDC, GetClRect(hwnd));
    }

    export void AdjustRight(HWND const, PIXEL const);
    export void AdjustLeft (HWND const, PIXEL const);

    export PixelRect CalcWindowRect(PixelRect const, DWORD const);

    export bool MoveWindowAbsolute(HWND const, PixelRect  const &, bool const);
    export bool MoveWindowAbsolute(HWND const, PixelPoint const &, bool const);
};
