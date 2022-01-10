// Direct2D.h
//
// Toolbox\Direct2D

#pragma once

#include <string.h>
#include "d2d1helper.h"
#include "PixelTypes.h"

using std::wstring;

struct IDWriteFactory;

inline void SafeRelease(auto **ppInterfaceToRelease)
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

    D2D_driver() = default;
    virtual ~D2D_driver();

    void Initialize(HWND const);
    bool StartFrame();
    void SetStdFontSize(float const);
    void DisplayText(fPixelRect const &, wstring const &, D2D1::ColorF const, IDWriteTextFormat * = nullptr) const;
    void EndFrame();
    void ShutDown();
    void DrawRectangle      (fPixelRect    const &, D2D1::ColorF const, fPixel const) const;
    void FillRectangle      (fPixelRect    const &, D2D1::ColorF const) const;
    void FillGradientRect   (fPixelRect    const &, D2D1::ColorF const, D2D1::ColorF const) const;
    void FillGradientEllipse(fPixelEllipse const &, D2D1::ColorF const, D2D1::ColorF const) const;
    void FillGradientCircle (fPixelCircle  const &, D2D1::ColorF const, D2D1::ColorF const) const;
    void DrawLine           (fPixelPoint   const &, fPixelPoint const &, fPixel const, D2D1::ColorF const) const;
    void FillCircle         (fPixelCircle  const &, D2D1::ColorF const) const;
    void DrawCircle         (fPixelCircle  const &, D2D1::ColorF const, fPixel const) const;
    void FillEllipse        (fPixelEllipse const &, D2D1::ColorF const) const;
    void DrawEllipse        (fPixelEllipse const &, D2D1::ColorF const, fPixel const) const;
    void FillArrow          (fPixelPoint   const, fPixelPoint const, fPixel const, fPixel const, D2D1::ColorF const) const;
    void FillDiamond        (fPixelPoint   const, fPixel const, D2D1::ColorF const) const;
    void Resize             (int const, int const);
    void FillBackground     (D2D1::ColorF const) const;

    fPixelRectSize GetClRectSize() const;
    fPixel         GetClRectWidth() const;
    fPixel         GetClRectHeight() const;

    IDWriteTextFormat * NewTextFormat(float const) const;

private:
    HWND m_hwnd { nullptr };

    mutable HRESULT m_hr { 0 };

    ID2D1Factory          * m_pD2DFactory    { nullptr };
    ID2D1HwndRenderTarget * m_pRenderTarget  { nullptr };
    IDWriteFactory        * m_pDWriteFactory { nullptr };
    IDWriteTextFormat     * m_pTextFormat    { nullptr };

    ID2D1SolidColorBrush * createBrush(D2D1::ColorF const) const;
    void createResources();
    void discardResources();

    friend D2D1_POINT_2F convertD2D(fPixelPoint   const &);
    friend D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);
};
