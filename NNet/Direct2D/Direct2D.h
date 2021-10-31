// Direct2D.h
//
// Direct2D

#pragma once

#include <string.h>
#include "d2d1helper.h"
#include "PixelTypes.h"

using std::wstring;

struct IDWriteFactory;

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

D2D1_POINT_2F convertD2D(fPixelPoint   const &);
D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);

class D2D_driver
{
public:

    D2D_driver();
    virtual ~D2D_driver();

    void Initialize(HWND const);
    bool StartFrame(HDC const);
    void SetStdFontSize(float const);
    void DisplayText(fPixelRect const &, wstring const &, D2D1::ColorF const, IDWriteTextFormat * = nullptr) const;
    void EndFrame();
    void ShutDown();
    void FillRectangle      (fPixelRect    const &, D2D1::ColorF const) const;
    void FillGradientRect   (fPixelRect    const &, D2D1::ColorF const, D2D1::ColorF const) const;
    void FillGradientEllipse(fPixelEllipse const &, D2D1::ColorF const, D2D1::ColorF const) const;
    void FillGradientCircle (fPixelCircle  const &, D2D1::ColorF const, D2D1::ColorF const) const;
    void DrawLine   (fPixelPoint const &, fPixelPoint const &, fPixel const, D2D1::ColorF const) const;
    void FillCircle (fPixelCircle  const &, D2D1::ColorF const) const;
    void DrawCircle (fPixelCircle  const &, D2D1::ColorF const, fPixel const) const;
    void FillEllipse(fPixelEllipse const &, D2D1::ColorF const) const;
    void DrawEllipse(fPixelEllipse const &, D2D1::ColorF const, fPixel const) const;
    void FillArrow(fPixelPoint const, fPixelPoint const, fPixel const, fPixel const, D2D1::ColorF const) const;
    void FillDiamond(fPixelPoint const, fPixel const, D2D1::ColorF const) const;
    void Resize(int const, int const);

    fPixelRectSize const GetClRectSize() const;

    IDWriteTextFormat * NewTextFormat(float const) const;

private:
    HWND m_hwnd;

    mutable HRESULT m_hr;

    ID2D1Factory          * m_pD2DFactory;
    ID2D1HwndRenderTarget * m_pRenderTarget;
    IDWriteFactory        * m_pDWriteFactory;
    IDWriteTextFormat     * m_pTextFormat;

    ID2D1SolidColorBrush * createBrush(D2D1::ColorF const) const;
    void createResources();
    void discardResources();

    friend D2D1_POINT_2F convertD2D(fPixelPoint   const &);
    friend D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);

    //fPixel const getValueA
    //(
    //    bool        const bScaleType,
    //    fPixelPoint const & p
    //) const
    //{
    //    return bScaleType ? p.GetY() : p.GetX();
    //}

    //fPixel const getValueB
    //(
    //    bool        const bScaleType,
    //    fPixelPoint const & p
    //) const
    //{
    //    return bScaleType ? p.GetX() : p.GetY();
    //}

    //fPixelPoint const makefPixelPnt
    //(
    //    bool   const bScaleType, 
    //    fPixel const fPixA, 
    //    fPixel const fPixB
    //) const
    //{
    //    return bScaleType ? fPixelPoint(fPixB, fPixA) : fPixelPoint(fPixA, fPixB);
    //}

    //void displayTick
    //(
    //    bool         const bScaleType,
    //    bool         const bOrientation,
    //    fPixelPoint  const fPixPntStart, 
    //    fPixel       const fTickA,
    //    fPixel       const fTickExt,
    //    D2D1::ColorF const colF
    //) const
    //{
    //    fPixel      const fDir(bOrientation ? -fTickExt : fTickExt);
    //    fPixelPoint const fTickStart { makefPixelPnt(bScaleType, fTickA, getValueB(bScaleType, fPixPntStart)) };
    //    fPixelPoint const fTickEnd   { makefPixelPnt(bScaleType, fTickA, getValueB(bScaleType, fPixPntStart) + fDir) };
    //    DrawLine(fTickStart, fTickEnd, 1._fPixel, colF);
    //}

    //void displayTicks() const
    //{
    //    static fPixel fLongTick  (10._fPixel);
    //    static fPixel fMiddleTick( 7._fPixel);
    //    static fPixel fSmallTick ( 5._fPixel);

    //    int iNrOfTicks { static_cast<int>((m_logEnd - m_logStart) / m_logTickDist) };
    //    for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
    //    {
    //        LogUnits lu    { m_logStart + iRun * m_logTickDist };
    //        fPixel   fTick { (iRun % 5 == 0) ? fLongTick : (iRun % 2 == 0) ? fMiddleTick : fSmallTick};
    //        fPixel   fPixA { log2pix(lu) };
    //        displayTick(fPixA, fTick);
    //        if (iRun % 10 == 0)
    //            display
    //            (
    //                makefPixelPnt(fPixA, getValueB(m_fPixPntStart)), 
    //                to_wstring(static_cast<int>(round(lu / m_logReduction)))
    //            );
    //    }
    //}

};
