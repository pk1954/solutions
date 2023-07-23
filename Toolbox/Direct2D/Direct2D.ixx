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

import Color;
import Types;

using std::function;
using std::wstring;
using std::unique_ptr;

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
    void DisplayText         (fPixelRect    const&, wstring const&, ID2D1Brush   const&, IDWriteTextFormat* = nullptr) const;
    void DisplayText         (fPixelRect    const&, wstring const&, Color const,  IDWriteTextFormat* = nullptr) const;
    void DisplayText         (fPixelRect    const&, wstring const&,                      IDWriteTextFormat* = nullptr) const;
    void DrawRectangle       (fPixelRect    const&, Color const, fPixel const)                                  const;
    void FillRectangle       (fPixelRect    const&, Color const)                                                const;
    void ClearRectangle      (fPixelRect    const&)                                                const;
    void FillGradientRect    (fPixelRect    const&, Color const, Color const)                            const;
    void FillGradientEllipse (fPixelEllipse const&, Color const, Color const)                            const;
    void FillGradientCircle  (fPixelCircle  const&, Color const, Color const)                            const;
    void DrawLine            (fPixelLine    const&,                     fPixel const, ID2D1Brush   const&)             const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const&, fPixel const, ID2D1Brush   const&)             const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const&, fPixel const, Color const)              const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const&, fPixel const)                                  const;
    void FillCircle          (fPixelCircle  const&, ID2D1Brush* const)                                                 const;
    void FillCircle          (fPixelCircle  const&, Color const)                                                const;
    void FillCircle          (fPixelCircle  const&)                                                                    const;
    void DrawCircle          (fPixelCircle  const&, ID2D1Brush * const, fPixel const)                                  const;
    void DrawCircle          (fPixelCircle  const&, Color const, fPixel const)                                  const;
    void DrawCircle          (fPixelCircle  const&,                     fPixel const)                                  const;
    void FillEllipse         (fPixelEllipse const&, ID2D1Brush* const)                                                 const;
    void FillEllipse         (fPixelEllipse const&, Color const)                                                const;
    void FillEllipse         (fPixelEllipse const&)                                                                    const;
    void DrawEllipse         (fPixelEllipse const&, ID2D1Brush* const,  fPixel const)                                  const;
    void DrawEllipse         (fPixelEllipse const&, Color const, fPixel const)                                  const;
    void DrawEllipse         (fPixelEllipse const&,                     fPixel const)                                  const;
    void FillArrow           (fPixelPoint const, fPixelPoint const, fPixel const, fPixel const, Color const)    const;
    void FillDiamond         (fPixelPoint const, fPixel const, Color const) const;
    void DrawRoundedRectangle(fPixelRect const&, Color const, fPixel const, fPixel const) const;
    void FillRoundedRectangle(fPixelRect const&, Color const, fPixel const) const;
    void UpDownArrow         (bool  const, fPixelRect  const &, Color const) const;
    void SetRotation         (float const, fPixelPoint const &) const;
    void Reset               () const;
    void Resize              ();

    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&,                        fPixel const) const;
    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, Color const,    fPixel const) const;
    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, ID2D1SolidColorBrush*, fPixel const) const;

    fPixelRectSize GetClRectSize  () const;
    fPixel         GetClRectWidth () const;
    fPixel         GetClRectHeight() const;

    IDWriteTextFormat * NewTextFormat(float const) const;

    ID2D1SolidColorBrush* CreateBrush(Color const) const;
        
    Color SetForegroundColor(Color const);
    Color SetBackgroundColor(Color const);

    Color GetForegroundColor() const;
    Color GetBackgroundColor() const;

private:
    HWND m_hwnd { nullptr };

    mutable HRESULT m_hr { 0 };

    mutable ID2D1GeometrySink * m_pSink        { nullptr };

    ID2D1Factory          * m_pD2DFactory      { nullptr };
    ID2D1HwndRenderTarget * m_pRenderTarget    { nullptr };
    IDWriteFactory        * m_pDWriteFactory   { nullptr };
    IDWriteTextFormat     * m_pTextFormat      { nullptr };
    ID2D1SolidColorBrush  * m_pBrushForeground { nullptr };
    ID2D1SolidColorBrush  * m_pBrushBackground { nullptr };

    void createResources();
    void discardResources();
    bool startFrame();
    void endFrame();

    ID2D1GradientStopCollection * simpleGradientStopCollection(Color const, Color const) const;

    friend D2D1_POINT_2F convertD2D(fPixelPoint   const &);
    friend D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);
};
