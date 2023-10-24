// GDI_driver.cpp
//
// GDI


#include <string.h>
#include "util.h"
#include "win32_util.h"
#include "PixelTypes.h"
#include "GDI_driver.h"

GDI_driver::GDI_driver() 
{
}

GDI_driver::~GDI_driver()
{
}

void GDI_driver::Initialize
(
	HWND  const hwndApp, 
	ULONG const ulModelWidth, 
	ULONG const ulModelHeight, 
	BOOL  const bHexagon 
) 
{
}

void GDI_driver::ShutDown()
{

}

PixelRect GDI_driver::CalcGraphicsRect(std::wstring const & wstr)
{
	RECT rect{ 0, 0, 0, 0 };
	return Util::RECT2PixelRect(rect);
}

void GDI_driver::DisplayGraphicsText(PixelRect const & pixRect, std::wstring const & wstr, DWORD const, COLORREF const)
{
	RECT rect(Util::PixelRect2RECT(pixRect));			  
}

bool GDI_driver::StartFrame(HWND const hwnd, HDC const hDC)
{
	m_hDC = hDC;
	return true;
}

void GDI_driver::SetFontSize(PIXEL const nPointSize)
{
}

RECT GDI_driver::getRECT(PixelPoint const ptPos, float const fPixSizeHalf)
{
    float const fPtPosx = static_cast<float>(ptPos.GetXvalue());
    float const fPtPosy = static_cast<float>(ptPos.GetYvalue());

    PIXEL pixStartX { static_cast<PIXEL>(CastToLong(fPtPosx - fPixSizeHalf)) };
    PIXEL pixStartY { static_cast<PIXEL>(CastToLong(fPtPosy - fPixSizeHalf)) };
    PIXEL pixEndX   { static_cast<PIXEL>(CastToLong(fPtPosx + fPixSizeHalf)) };
	PIXEL pixEndY   { static_cast<PIXEL>(CastToLong(fPtPosy + fPixSizeHalf)) };

	PixelRect const pixRect{ pixStartX, pixStartY, pixEndX, pixEndY };
	RECT      const rect = Util::PixelRect2RECT(pixRect);
	return rect;
}

void GDI_driver::AddIndividual(PixelPoint const ptPos, COLORREF const color, float const fPixSizeHalf)
{
    SetBkColor(m_hDC, color);
    Util::FastFill(m_hDC, getRECT(ptPos, fPixSizeHalf));
}

void GDI_driver::AddBackGround(PixelPoint const ptPos, COLORREF const color, float const fPixSize)
{
    SetBkColor(m_hDC, color);
    Util::FastFill(m_hDC, getRECT(ptPos, fPixSize / 2));
}

void GDI_driver::RenderTranspRect
(
	PixelRect    const & rectTransparent, 
	unsigned int const   uiALpha,
	COLORREF     const   color
)
{
	if (rectTransparent.IsNotEmpty())
	{
	}
}

void GDI_driver::RenderBackground()
{
}

void GDI_driver::RenderForegroundObjects()
{
}

void GDI_driver::EndFrame(HWND const)
{
}
