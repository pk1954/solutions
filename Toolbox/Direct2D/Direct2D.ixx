// Direct2D.ixx
//
// Toolbox\Direct2D

module;

#include <functional>
#include <memory>
#include <string>
#include "dwrite.h"
#include "d2d1helper.h"
#include <Windows.h>

export module Direct2D;

import Types;

using std::function;
using std::wstring;
using std::unique_ptr;

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

    static unique_ptr<D2D_driver> Create(HWND const hwnd);

    void Display(function<void()>);
    void ShutDown();
    void SetStdFontSize(float const);
    void DisplayText(fPixelRect const&, wstring const&, ID2D1Brush   const&, IDWriteTextFormat* = nullptr)  const;
    void DisplayText(fPixelRect const&, wstring const&, D2D1::ColorF const,  IDWriteTextFormat* = nullptr)  const;
    void DisplayText(fPixelRect const&, wstring const&,                      IDWriteTextFormat* = nullptr)  const;
    void DrawRectangle       (fPixelRect    const&, D2D1::ColorF const, fPixel const)                       const;
    void FillRectangle       (fPixelRect    const&, D2D1::ColorF const)                                     const;
    void FillGradientRect    (fPixelRect    const&, D2D1::ColorF const, D2D1::ColorF const)                 const;
    void FillGradientEllipse (fPixelEllipse const&, D2D1::ColorF const, D2D1::ColorF const)                 const;
    void FillGradientCircle  (fPixelCircle  const&, D2D1::ColorF const, D2D1::ColorF const)                 const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const &, fPixel const, ID2D1Brush   const&) const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const &, fPixel const, D2D1::ColorF const)  const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const &, fPixel const)                      const;
    void FillCircle          (fPixelCircle  const&, ID2D1Brush* const)                                      const;
    void FillCircle          (fPixelCircle  const&, D2D1::ColorF const)                                     const;
    void FillCircle          (fPixelCircle  const&)                                                         const;
    void DrawCircle          (fPixelCircle  const&, ID2D1Brush * const, fPixel const)                       const;
    void DrawCircle          (fPixelCircle  const&, D2D1::ColorF const, fPixel const)                       const;
    void DrawCircle          (fPixelCircle  const&,                     fPixel const)                       const;
    void FillEllipse         (fPixelEllipse const&, ID2D1Brush* const)                                      const;
    void FillEllipse         (fPixelEllipse const&, D2D1::ColorF const)                                     const;
    void FillEllipse         (fPixelEllipse const&)                                                         const;
    void DrawEllipse         (fPixelEllipse const&, ID2D1Brush* const,  fPixel const)                       const;
    void DrawEllipse         (fPixelEllipse const&, D2D1::ColorF const, fPixel const)                       const;
    void DrawEllipse         (fPixelEllipse const&,                     fPixel const)                       const;
    void FillArrow           (fPixelPoint const, fPixelPoint const, fPixel const, fPixel const, D2D1::ColorF const) const;
    void FillDiamond         (fPixelPoint const, fPixel const, D2D1::ColorF const) const;
    void FillBackground      (D2D1::ColorF const) const;
    void DrawRoundedRectangle(fPixelRect const&, D2D1::ColorF const, fPixel const, fPixel const) const;
    void FillRoundedRectangle(fPixelRect const&, D2D1::ColorF const, fPixel const) const;
    void UpDownArrow         (bool  const, fPixelRect  const &, D2D1::ColorF const) const;
    void SetRotation         (float const, fPixelPoint const &) const;
    void Reset               () const;
    void Resize              ();

    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&,                         fPixel const) const;
    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, D2D1::ColorF const,     fPixel const) const;
    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, ID2D1SolidColorBrush *, fPixel const) const;

    fPixelRectSize GetClRectSize  () const;
    fPixel         GetClRectWidth () const;
    fPixel         GetClRectHeight() const;

    IDWriteTextFormat * NewTextFormat(float const) const;

    ID2D1SolidColorBrush* CreateBrush(D2D1::ColorF const) const;
        
    D2D1::ColorF SetColor(D2D1::ColorF const);

private:
    HWND m_hwnd { nullptr };

    mutable HRESULT m_hr { 0 };

    mutable ID2D1GeometrySink * m_pSink      { nullptr };

    ID2D1Factory          * m_pD2DFactory    { nullptr };
    ID2D1HwndRenderTarget * m_pRenderTarget  { nullptr };
    IDWriteFactory        * m_pDWriteFactory { nullptr };
    IDWriteTextFormat     * m_pTextFormat    { nullptr };
    ID2D1SolidColorBrush  * m_pBrush         { nullptr };
    D2D1::ColorF            m_color          { D2D1::ColorF::Black };

    void createResources();
    void discardResources();
    bool startFrame();
    void endFrame();

    ID2D1GradientStopCollection * simpleGradientStopCollection(D2D1::ColorF const, D2D1::ColorF const) const;

    friend D2D1_POINT_2F convertD2D(fPixelPoint   const &);
    friend D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);
};
