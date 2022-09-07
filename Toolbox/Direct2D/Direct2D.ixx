// Direct2D.ixx
//
// Toolbox\Direct2D

module;

#include <string>
#include "dwrite.h"
#include "d2d1helper.h"

export module Direct2D;

import Types;

using std::wstring;

export class D2D1::ColorF;

export inline void SafeRelease(auto **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

export D2D1_POINT_2F convertD2D(fPixelPoint   const &);
export D2D1_RECT_F   convertD2D(fPixelRect    const &);
export D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);

export class D2D_driver
{
public:

    D2D_driver() = default;
    virtual ~D2D_driver();

    void InitWindow(HWND const);
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
    void Resize             (PIXEL const, PIXEL const);
    void FillBackground     (D2D1::ColorF const) const;
    void UpDownArrow        (bool const, fPixelRect const &, D2D1::ColorF const) const;

    void DrawLine(fPixelPoint const &, fPixelPoint const &, fPixel const, ID2D1SolidColorBrush *) const;

    fPixelRectSize GetClRectSize() const;
    fPixel         GetClRectWidth() const;
    fPixel         GetClRectHeight() const;

    ID2D1SolidColorBrush * CreateBrush(D2D1::ColorF const) const;
    IDWriteTextFormat    * NewTextFormat(float const) const;

private:
    HWND m_hwnd { nullptr };

    mutable HRESULT m_hr { 0 };

    ID2D1Factory          * m_pD2DFactory    { nullptr };
    ID2D1HwndRenderTarget * m_pRenderTarget  { nullptr };
    IDWriteFactory        * m_pDWriteFactory { nullptr };
    IDWriteTextFormat     * m_pTextFormat    { nullptr };

    void createResources();
    void discardResources();

    ID2D1GradientStopCollection * simpleGradientStopCollection(D2D1::ColorF const, D2D1::ColorF const) const;

    friend D2D1_POINT_2F convertD2D(fPixelPoint   const &);
    friend D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);
};
