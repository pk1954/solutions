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
struct IDWriteFactory;
struct IDWriteTextFormat;

template<class Interface>
inline void SafeRelease( Interface **ppInterfaceToRelease )
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

    void Initialize( HWND const );
    bool StartFrame( HWND const, HDC const );
    void SetStdFontSize( float const );
    void DisplayText( PixelRect const &, std::wstring const &, COLORREF const, IDWriteTextFormat * = nullptr );
    void EndFrame( HWND const );
    void ShutDown( );
    void DrawLine( fPixelPoint const &, fPixelPoint const &, fPIXEL const, COLORREF const );
    void DrawPolygon( int const, fPixelPoint const, COLORREF const, fPIXEL const );
    void DrawCircle( fPixelPoint const, COLORREF const, fPIXEL const );
    void Resize( int const, int const );

    IDWriteTextFormat * NewTextFormat( float const );
    void DeleteTextFormat( IDWriteTextFormat ** );

private:
    HWND    m_hwnd;
    HRESULT m_hr;

    ID2D1Factory          * m_pD2DFactory;
    ID2D1HwndRenderTarget * m_pRenderTarget;
    IDWriteFactory        * m_pDWriteFactory;
    IDWriteTextFormat     * m_pStdTextFormat;

    ID2D1SolidColorBrush * createBrush( COLORREF const );
    void createResources( );
    void discardResources( );
};
