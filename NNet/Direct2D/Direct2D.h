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
    bool StartFrame( HDC const );
    void SetStdFontSize( float const );
    void DisplayText( PixelRect const &, std::wstring const &, D2D1::ColorF const, IDWriteTextFormat * = nullptr ) const;
    void EndFrame( );
    void ShutDown( );
    void DrawRectangle   ( fPixelRect const &, D2D1::ColorF const ) const;
    void DrawGradientRect( fPixelRect const &, D2D1::ColorF const, D2D1::ColorF const ) const;
    void DrawLine( fPixelPoint const &, fPixelPoint const &, fPIXEL const, D2D1::ColorF const ) const;
    void DrawCircle ( fPixelCircle  const &, D2D1::ColorF const ) const;
    void DrawEllipse( fPixelEllipse const &, D2D1::ColorF const ) const;
    void DrawArrow( fPixelPoint const, fPixelPoint const, fPIXEL const, fPIXEL const, D2D1::ColorF const ) const;
    void DrawDiamond( fPixelPoint const, fPIXEL const, D2D1::ColorF const ) const;
    void Resize( int const, int const );

    fPixelRectSize const GetClRectSize( ) const;

    IDWriteTextFormat * NewTextFormat( float const ) const;

private:
    HWND m_hwnd;

    mutable HRESULT m_hr;

    ID2D1Factory          * m_pD2DFactory;
    ID2D1HwndRenderTarget * m_pRenderTarget;
    IDWriteFactory        * m_pDWriteFactory;
    IDWriteTextFormat     * m_pTextFormat;

    ID2D1SolidColorBrush * createBrush( D2D1::ColorF const ) const;
    void createResources( );
    void discardResources( );
};
