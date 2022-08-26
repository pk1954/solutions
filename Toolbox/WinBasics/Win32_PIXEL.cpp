// Win32_PIXEL.cpp 
//
// Toolbox\WinBasics

module;

#include <iostream>
#include "Windows.h"

module Win32_PIXEL;

import PixelTypes;
import Script;

using std::wostream;

RECT Util::ScrReadRECT(Script & script)
{
    RECT rect;
    rect.left   = script.ScrReadLong();
    rect.top    = script.ScrReadLong();
    rect.right  = script.ScrReadLong();
    rect.bottom = script.ScrReadLong();
    return rect;
}

wostream & Util::operator << (wostream & out, RECT const & rect)
{
    out << rect.left << L' ' << rect.top << L' ' << rect.right << L' ' << rect.bottom;
    return out;
}

// CalcWindowRect: Calculates the required size of the window rectangle, based on the desired client-rectangle size. 

PixelRect Util::CalcWindowRect(PixelRect pixRect, DWORD const dwStyle)
{
	RECT rect = Util::PixelRect2RECT(pixRect);
	(void)AdjustWindowRect(&rect, dwStyle, false);	
	pixRect = Util::RECT2PixelRect(rect);
	return pixRect;
}

void Util::AdjustRight(HWND const hwnd, PIXEL const pixYpos)
{
	HWND  const hwndParent     = GetParent(hwnd);
	PIXEL const pixWidthParent = GetClientWindowWidth(hwndParent);
	PIXEL const pixWidth       = GetWindowWidth(hwnd);
	PIXEL const pixHeight      = GetWindowHeight(hwnd);
	MoveWindow(hwnd, (pixWidthParent - pixWidth), pixYpos, pixWidth, pixHeight, true);
	(void)BringWindowToTop(hwnd);
}

void Util::AdjustLeft(HWND const hwnd, PIXEL const pixYpos)
{
	PixelRectSize pnt = GetWindowSize(hwnd);
	MoveWindow(hwnd, 0_PIXEL, pixYpos, pnt.GetX(), pnt.GetY(), true);
	(void)BringWindowToTop(hwnd);
}

bool Util::MoveWindowAbsolute  // move window to given screen coordinates and set size
(
	HWND      const   hwnd,
	PixelRect const & pixRect,
	bool      const   bRepaint
)
{
	HWND const hwndParent { GetAncestor(hwnd, GA_PARENT) };
	PixelPoint pixPoint{ pixRect.GetStartPoint() };
	bool       bRes;

	if (hwndParent)
		pixPoint = Screen2Client(hwndParent, pixPoint);

	bRes = MoveWindow(hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint);

	if (GetWindowSize(hwnd) != pixRect.GetSize())   // can happen in strange situations
		bRes = MoveWindow(hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint);

	return bRes;
}

bool Util::MoveWindowAbsolute  // move window to given screen coordinates 
(
	HWND       const   hwnd,
	PixelPoint const & pixPos,
	bool       const   bRepaint
)
{
	return MoveWindowAbsolute(hwnd, PixelRect{ pixPos, GetWindowSize(hwnd) }, bRepaint);
}
