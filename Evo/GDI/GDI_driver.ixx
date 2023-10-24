// GDI_driver.h
//
// GDI

module;

#include <string.h>
#include "Windows.h"
#include "PixelTypes.h"
#include "win32_graphicsInterface.h"

export module GDI_driver;

import Win32_Util;

export class GDI_driver : public GraphicsInterface
{
public:

    GDI_driver();
    ~GDI_driver();
    virtual void      Initialize(HWND const, ULONG const, ULONG const, BOOL const);
    virtual bool      StartFrame(HWND const, HDC const);
    virtual void      SetFontSize(PIXEL const);
    virtual void      AddIndividual(PixelPoint const, COLORREF const, float const);
    virtual void      AddBackGround(PixelPoint const, COLORREF const, float const);
    virtual void      RenderForegroundObjects();
    virtual void      RenderBackground();
    virtual void      RenderTranspRect(PixelRect const&, unsigned int const, COLORREF const);
    virtual void      DisplayGraphicsText(PixelRect const&, std::wstring const&, DWORD const, COLORREF const);
    virtual PixelRect CalcGraphicsRect(std::wstring const&);
    virtual void      EndFrame(HWND const);
    virtual void      ShutDown();
    virtual void      StartPipeline(fPixelPoint const&, fPixelPoint const&, fPIXEL const, COLORREF const) {};
    virtual void      AddPipelinePoint(fPixelPoint const&, COLORREF const) {};
    virtual void      RenderPipeline() {};

private:
    RECT getRECT(PixelPoint const, float const);

    HDC    m_hDC;
    HBRUSH m_brush;
};
