// Direct2D.h
//
// Direct2D

#pragma once

#include <string.h>
#include "PixelTypes.h"

using std::wstring;

struct ID2D1Factory;
struct ID2D1HwndRenderTarget;
struct ID2D1SolidColorBrush;
struct ID2D1PathGeometry;
struct ID2D1GeometrySink;

template<class Interface>
inline void SafeRelease(
    Interface **ppInterfaceToRelease
)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

class D2D_driver
{
public:

    D2D_driver();
    ~D2D_driver();
    void      Initialize( HWND const, ULONG const, ULONG const, BOOL const );
    bool      StartFrame( HWND const, HDC const );
    void      SetFontSize( PIXEL const );
    void      AddIndividual( PixelPoint const, COLORREF const, float const );    
    void      AddBackGround( PixelPoint const, COLORREF const, float const );    
    void      RenderForegroundObjects( );
    void      RenderBackground( );
    void      RenderTranspRect( PixelRect const &, unsigned int const, COLORREF const );
    void      DisplayGraphicsText( PixelRect const &, std::wstring const &, DWORD const, COLORREF const );
    PixelRect CalcGraphicsRect( wstring const & );
    void      EndFrame( HWND const );
    void      ShutDown( );
    void      StartPipeline( fPixelPoint const &, fPixelPoint const &, fPIXEL const, COLORREF const );
    void      AddPipelinePoint( fPixelPoint const &, COLORREF const );
    void      RenderPipeline( );
    void	  AddfPixelLine( fPixelPoint const &, fPixelPoint const &, fPIXEL const, COLORREF const );
    void      DrawPolygon( int const, fPixelPoint const, COLORREF const, fPIXEL const );
    void      Resize( int const, int const );

private:
    HRESULT m_hr;

    ID2D1Factory          * m_pD2DFactory;
    ID2D1HwndRenderTarget * m_pRenderTarget;
    ID2D1SolidColorBrush  * m_pBlackBrush;
    ID2D1PathGeometry     * m_pPathGeometry;
    ID2D1GeometrySink     * m_pSink;

    fPixelPoint m_fOrtho;
    fPixelPoint m_fp1;
    fPixelPoint m_fp2;

};
