// Direct2D.ixx
//
// Toolbox\Direct2D

module;

#include "dwrite.h"
#include "d2d1helper.h"

export module Direct2D;

import std;
import Color;
import Types;
import WinBasics;

using std::vector;
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

class D2D_driver;

export using UPD2D = unique_ptr<D2D_driver>;

export class D2D_driver
{
public:

    D2D_driver() = default;
    virtual ~D2D_driver();

    static UPD2D Create(HWND const hwnd);

    void Display(function<void()>);
    void ShutDown();
    void Push();
    void Pop();
    void SetStdFontSize      (float const);
    void Resize              (PIXEL const, PIXEL const);
    void DrawRectangle       (fPixelRect    const&, Color const, fPixel const)                                         const;
    void FillRectangle       (fPixelRect    const&, Color const)                                                       const;
    void FillRectangle       (fPixelRect    const&, BrushHandle const)                                                 const;
    void ClearRectangle      (fPixelRect    const&)                                                                    const;
    void FillGradientRect    (fPixelRect    const&, Color const, Color const)                                          const;
    void FillGradientEllipse (fPixelEllipse const&, Color const, Color const)                                          const;
    void FillGradientCircle  (fPixelCircle  const&, Color const, Color const)                                          const;
    void DrawLine            (fPixelLine    const&,                     fPixel const, ID2D1Brush   const&)             const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const&, fPixel const, ID2D1Brush   const&)             const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const&, fPixel const, Color const)                     const;
    void DrawLine            (fPixelPoint   const&, fPixelPoint const&, fPixel const)                                  const;
    void FillCircle          (fPixelCircle  const&, ID2D1Brush* const)                                                 const;
    void FillCircle          (fPixelCircle  const&, Color const)                                                       const;
    void FillCircle          (fPixelCircle  const&)                                                                    const;
    void DrawCircle          (fPixelCircle  const&, ID2D1Brush* const, fPixel const)                                   const;
    void DrawCircle          (fPixelCircle  const&, Color const,       fPixel const)                                   const;
    void DrawCircle          (fPixelCircle  const&,                    fPixel const)                                   const;
    void FillEllipse         (fPixelEllipse const&, ID2D1Brush* const)                                                 const;
    void FillEllipse         (fPixelEllipse const&, Color const)                                                       const;
    void FillEllipse         (fPixelEllipse const&)                                                                    const;
    void DrawEllipse         (fPixelEllipse const&, ID2D1Brush* const,  fPixel const)                                  const;
    void DrawEllipse         (fPixelEllipse const&, Color const,        fPixel const)                                  const;
    void DrawEllipse         (fPixelEllipse const&,                     fPixel const)                                  const;
    void FillArrow           (fPixelPoint   const,  fPixelPoint const,  fPixel const, fPixel const, Color const)       const;
    void FillDiamond         (fPixelPoint   const,  fPixel const,       Color  const)                                  const;
    void DrawRoundedRectangle(fPixelRect    const&, Color const,        fPixel const, fPixel const)                    const;
    void FillRoundedRectangle(fPixelRect    const&, Color const,        fPixel const)                                  const;
    void UpDownArrow         (bool          const,  fPixelRect  const &, Color const)                                  const;
    void SetRotation         (Degrees       const,  fPixelPoint const &)                                               const;
    void Reset               () const;

    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&,              fPixel const) const;
    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, Color const, fPixel const) const;
    void DrawBezier(fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, fPixelPoint const&, BrushHandle, fPixel const) const;

    void DisplayText(fPixelRect const&, wstring const&, ID2D1Brush const&, TextFormatHandle = nullptr) const;
    void DisplayText(fPixelRect const&, wstring const&, Color const,       TextFormatHandle = nullptr) const;
    void DisplayText(fPixelRect const&, wstring const&,                    TextFormatHandle = nullptr) const;

    void DisplayText(wstring const&, ID2D1Brush const&, TextFormatHandle = nullptr) const;
    void DisplayText(wstring const&, Color       const, TextFormatHandle = nullptr) const;
    void DisplayText(wstring const&,                    TextFormatHandle = nullptr) const;

    fPixelRectSize CalcRect(wstring const&) const;

    fPixelRect     GetClRect      () const { return Convert2fPixelRect    (::GetClPixelRect      (m_hwnd)); }
    fPixelRectSize GetClRectSize  () const { return Convert2fPixelRectSize(::GetClRectSize       (m_hwnd)); }
    fPixel         GetClRectWidth () const { return Convert2fPixel        (::GetClientWindowWidth(m_hwnd)); }
    fPixel         GetClRectHeight() const { return Convert2fPixel        (::GetClientWindowHeight(m_hwnd));}

    TextFormatHandle NewTextFormat(float const) const;
    BrushHandle      CreateBrushHandle(Color const) const;
        
    void SetForegroundColor(Color const);
    void SetBackgroundColor(Color const);

    Color GetForegroundColor() const;
    Color GetBackgroundColor() const;

    float GetFontSize() const { return m_pTextFormat->GetFontSize(); }

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

    vector<D2D1_MATRIX_3X2_F> m_transformStack;

    ID2D1SolidColorBrush* createBrush(Color const) const;
    
    void createResources();
    void discardResources();
    bool startFrame();
    void endFrame();

    ID2D1GradientStopCollection * simpleGradientStopCollection(Color const, Color const) const;

    friend D2D1_POINT_2F convertD2D(fPixelPoint   const &);
    friend D2D1_ELLIPSE  convertD2D(fPixelEllipse const &);
};
