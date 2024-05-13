// Win32_PIXEL.cpp 
//
// Toolbox\WinBasics

module;

#include <cassert>
#include <iostream>
#include <Windows.h>

module Win32_PIXEL;

import Types;
import Script;
import IoConstants;

using std::wostream;

RECT ScrReadRECT(Script & script)
{
    RECT rect;
    rect.left   = script.ScrReadLong();
    rect.top    = script.ScrReadLong();
    rect.right  = script.ScrReadLong();
    rect.bottom = script.ScrReadLong();
    return rect;
}

wostream & operator<< (wostream & out, RECT const & rect)
{
    out << rect.left << SPACE << rect.top << SPACE << rect.right << SPACE << rect.bottom;
    return out;
}

// CalcWindowRect: Calculates the required size of the window rectangle, based on the desired client-rectangle size. 

PixelRect CalcWindowRect(PixelRect pixRect, DWORD const dwStyle)
{
	RECT rect = ::PixelRect2RECT(pixRect);
	(void)AdjustWindowRect(&rect, dwStyle, false);	
	pixRect = ::RECT2PixelRect(rect);
	return pixRect;
}

void AdjustRight(HWND const hwnd, PIXEL const pixYpos)
{
	HWND  const hwndParent     = GetParent(hwnd);
	PIXEL const pixWidthParent = GetClientWindowWidth(hwndParent);
	PIXEL const pixWidth       = GetWindowWidth(hwnd);
	PIXEL const pixHeight      = GetWindowHeight(hwnd);
	MoveWindow(hwnd, (pixWidthParent - pixWidth), pixYpos, pixWidth, pixHeight, true);
	(void)BringWindowToTop(hwnd);
}

void AdjustLeft(HWND const hwnd, PIXEL const pixYpos)
{
	PixelRectSize pnt = GetWindowSize(hwnd);
	MoveWindow(hwnd, 0_PIXEL, pixYpos, pnt.GetX(), pnt.GetY(), true);
	(void)BringWindowToTop(hwnd);
}

bool MoveWindowAbsolute  // move window to given screen coordinates and set size
(
	HWND      const   hwnd,
	PixelRect const & pixRect,
	bool      const   bRepaint
)
{
	HWND const hwndParent { GetAncestor(hwnd, GA_PARENT) };
	PixelPoint pixPoint   { pixRect.GetStartPoint() };
	bool       bRes;

	if (hwndParent)
		pixPoint = Screen2Client(hwndParent, pixPoint);

	bRes = MoveWindow(hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint);

	if (GetWindowSize(hwnd) != pixRect.GetSize())   // can happen in strange situations
		bRes = MoveWindow(hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint);

	return bRes;
}

bool MoveWindowAbsolute  // move window to given screen coordinates 
(
	HWND       const   hwnd,
	PixelPoint const & pixPos,
	bool       const   bRepaint
)
{
	return MoveWindowAbsolute(hwnd, PixelRect{ pixPos, GetWindowSize(hwnd) }, bRepaint);
}

static void arrangeVertical
(
	PixelRectSize const &size,
	PIXEL         const  pixFrameWidth,
	HWND          const  hwndTop,
	PIXEL         const  pixTopHeight,
	HWND          const  hwndBottom,
	PIXEL         const  pixBottomHeight
)
{
	PIXEL const pixWidthNet { size.GetX() - pixFrameWidth * 2 };
	assert(size.GetY() > pixTopHeight - pixFrameWidth * 3);
	assert(GetParent(hwndTop) == GetParent(hwndBottom));
	::MoveWindow
	(
		hwndTop,
		pixFrameWidth + 1_PIXEL, // Unclear why + 1_PIXEL
		pixFrameWidth + 1_PIXEL, // Unclear why + 1_PIXEL               
		pixWidthNet - 3_PIXEL,   // Unclear why -3_PIXEL 
		pixTopHeight - 1_PIXEL,  // Unclear why - 1_PIXEL
		true
	);
	::MoveWindow
	(
		hwndBottom,
		pixFrameWidth, 
		pixTopHeight + pixFrameWidth * 2, 
		pixWidthNet - 1_PIXEL,        // Unclear why - 1_PIXEL
		pixBottomHeight - 1_PIXEL,    // Unclear why - 1_PIXEL
		true
	);
}

void ArrangeVertical
(
	HWND  const hwndTop,
	PIXEL const pixTopHeight,
	HWND  const hwndBottom,
	PIXEL const pixFrameWidth
)
{
	if (!hwndTop || !hwndBottom)
		return;
	PixelRectSize const size { ::GetClRectSize(GetParent(hwndTop)) };
	PIXEL         const pixBottomHeight { size.GetY() - pixTopHeight  - pixFrameWidth * 3 };
	arrangeVertical(size, pixFrameWidth, hwndTop, pixTopHeight, hwndBottom, pixBottomHeight);
}

void ArrangeVertical
(
	HWND  const hwndTop,
	HWND  const hwndBottom,
	PIXEL const pixBottomHeight,
	PIXEL const pixFrameWidth
)
{
	if (!hwndTop || !hwndBottom)
		return;
	PixelRectSize const size         { ::GetClRectSize(GetParent(hwndTop)) };
	PIXEL         const pixTopHeight { size.GetY() - pixBottomHeight  - pixFrameWidth * 3 };
	arrangeVertical(size, pixFrameWidth, hwndTop, pixTopHeight, hwndBottom, pixBottomHeight);
}
