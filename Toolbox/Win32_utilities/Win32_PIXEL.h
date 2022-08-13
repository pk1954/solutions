// Win32_PIXEL.h
//
// Toolbox\Win32_utilities

#pragma once

#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include "Windows.h"
#include "BasicTypes.h"

import BoolOp;
import NamedType;
import MoreTypes;
import PixelTypes;

using std::wostringstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;

class Script;

namespace Util
{
    wostream & operator << (wostream &, RECT const &);

    RECT ScrReadRECT(Script &);

    inline bool CtrlKeyDown() { return GetAsyncKeyState(VK_CONTROL) & 0x01; }

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

    inline bool MoveWindow(HWND const hwnd, PixelRect const rect, bool const bRedraw)
    {
        return MoveWindow(hwnd, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), bRedraw);
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

    inline void SetWindowHeight(HWND const hwnd, PIXEL const newHeight, bool const bRedraw)
    {
        PixelRect rect { GetWindowRect(hwnd) };
        rect.SetHeight(newHeight);
        MoveWindow(hwnd, rect, bRedraw);
    }

    inline void SetWindowWidth(HWND const hwnd, PIXEL const newWidth, bool const bRedraw)
    {
        PixelRect rect { GetWindowRect(hwnd) };
        rect.SetWidth(newWidth);
        MoveWindow(hwnd, rect, bRedraw);
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

    void AdjustRight(HWND const, PIXEL const);
    void AdjustLeft (HWND const, PIXEL const);

    PixelRect CalcWindowRect(PixelRect const, DWORD const);

    bool MoveWindowAbsolute(HWND const, PixelRect  const &, bool const);
    bool MoveWindowAbsolute(HWND const, PixelPoint const &, bool const);
};
