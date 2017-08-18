// d3d_buffer.cpp
//

#include "stdafx.h"
#include <string.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include "win32_util.h"
#include "pixelPoint.h"
#include "d3d_system.h"
#include "d3d_buffer.h"

//lint -e534                                   ignoring return code 
//lint -esym( 613, D3dBuffer::m_d3d_device )   possible use of null pointer
//lint -e835                                   0 as left argument to << or |
//lint -e845                                   0 as right argument to << or |

static COLORREF const CLR_WHITE = D3DCOLOR_ARGB( 255, 255, 255, 255 );

D3DXFONT_DESC D3dBuffer::m_d3dx_font_desc =
{
    0,                         //  Height (will be set during initializition)
    0,                         //  Width
    FW_NORMAL,                 //  Weight
    1,                         //  MipLevels
    false,                     //  Italic
    DEFAULT_CHARSET,           //  CharSet
    OUT_DEFAULT_PRECIS,        //  OutputPrecision
    ANTIALIASED_QUALITY,       //  Quality
    DEFAULT_PITCH|FF_DONTCARE, //  PitchAndFamily
    L"Arial"                   //  FaceName
};

D3dBuffer::D3dBuffer( HWND const hWnd, ULONG const ulNrOfPoints ) :
    ulNrOfRects( ulNrOfPoints ),  // !!!!!!
    ulNrOfVertices( ulNrOfRects * 4 ),
    m_VertexBuffer( ulNrOfPoints ),
    m_RectBuffer( ulNrOfVertices )
{
    HRESULT hres;

    m_d3d        = D3dSystem::GetSystem( );
    m_d3d_device = m_d3d->GetDevice( );                                  assert( m_d3d_device != nullptr );
    hres = m_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );        assert( hres == D3D_OK );
    hres = m_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ); assert( hres == D3D_OK );
    hres = m_d3d_device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );          assert( hres == D3D_OK );

    hres = m_d3d_device->CreateVertexBuffer
    (
        sizeof(Vertex) * ulNrOfVertices,
        0,  //D3DUSAGE_DYNAMIC,
        D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
        D3DPOOL_MANAGED,   //D3DPOOL_DEFAULT,
        &m_d3d_vertexBuffer,
        nullptr
    );

    assert( hres == D3D_OK );

    hres = m_d3d_device->GetSwapChain( 0, &m_d3d_swapChain );   assert( hres == D3D_OK );
    m_dwAlphaBlendable = 0;
    m_dwSrcBlend       = 0;
    m_dwDstBlend       = 0;
    m_bStripMode       = TRUE;
    m_id3dx_font       = nullptr;
    m_hWnd             = hWnd;
    ResetFont( );
}

D3dBuffer::~D3dBuffer()
{
    try
    {
        ULONG const ulres = m_d3d_vertexBuffer->Release( );    assert( ulres == 0 );
    }
    catch ( ... )
    {
        exit( 1 );
    };

    m_d3d_vertexBuffer = nullptr;
    m_d3d_swapChain    = nullptr;
    m_d3d_device       = nullptr;
    m_id3dx_font       = nullptr;
    m_d3d              = nullptr;
    m_hWnd             = nullptr;
}

void D3dBuffer::D3D_DrawText( PixelRect rect, wstring const & wstr, D3DCOLOR col )
{
    assert( m_id3dx_font != nullptr );
    //lint -esym( 613, D3dBuffer::m_id3dx_font )  possible use of null pointer
    m_id3dx_font->DrawText
    ( 
        nullptr,           // pSprite
        wstr.c_str( ),     // pString
        -1,                // Count
        &rect,             // pRect
        DT_LEFT,           // Format
        col                // Color
    ); 
    //lint +esym( 613, D3dBuffer::m_id3dx_font ) 
}

// functions called per frame

void D3dBuffer::StartFrame( )
{
    HRESULT hres;

    m_d3d->ResetD3dSystem( m_hWnd );

    hres = m_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );               assert(hres == D3D_OK);
    hres = m_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );        assert(hres == D3D_OK);
    hres = m_d3d_device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );                 assert(hres == D3D_OK);

//lint -e620          suspicious constant (L or one?)
    hres = m_d3d_device->Clear( 0, nullptr, D3DCLEAR_TARGET, CLR_WHITE, 1.0f, 0 ); assert(hres == D3D_OK);
//lint +e620    
    m_d3d->SetTransform( m_hWnd );
    hres = m_d3d_device->BeginScene( );                                         assert(hres == D3D_OK);

    if ( m_bStripMode )
        m_VertexBuffer.ResetVertexBuffer();

    m_RectBuffer.ResetVertexBuffer();

    setFont( );   
}

void D3dBuffer::setFont( )
{
    HRESULT const hres = D3DXCreateFontIndirect( m_d3d_device, &m_d3dx_font_desc, &m_id3dx_font );   
    assert( hres == D3D_OK );
    assert( m_id3dx_font != nullptr );
}

void D3dBuffer::ResetFont( )
{
    //
    // To create a Windows friendly font using only a point size, an 
    // application must calculate the logical height of the font.
    // 
    // This is because functions like CreateFont() and CreateFontIndirect() 
    // only use logical units to specify height.
    //
    // Here's the formula to find the height in logical pixels:
    //
    //             -( point_size * LOGPIXELSY )
    //    height = ----------------------------
    //                          72
    //

    int const nPointSize = 9;
    HDC const hDC = GetDC( nullptr );
    int const iLogPixels = GetDeviceCaps( hDC, LOGPIXELSY );
    ReleaseDC( nullptr, hDC );
    m_d3dx_font_desc.Height = -( MulDiv( nPointSize, iLogPixels, 72 ) );
    setFont( );   
}

void D3dBuffer::AddVertex( PixelPoint const & ptCenter, DWORD const dwColor )
{
    m_VertexBuffer.AddVertex( static_cast<float>(ptCenter.x), static_cast<float>(ptCenter.y), dwColor );
}

void D3dBuffer::addRect2Buffer( float const fWest, float const fNorth, float const fEast, float const fSouth, DWORD const dwColor )
{    
    m_RectBuffer.AddVertex( fWest, fNorth, dwColor );
    m_RectBuffer.AddVertex( fEast, fNorth, dwColor );
    m_RectBuffer.AddVertex( fEast, fSouth, dwColor );
    m_RectBuffer.AddVertex( fWest, fSouth, dwColor );
}

void D3dBuffer::AddRect( PixelPoint const & ptPos, DWORD const dwColor, float const fPixSize )
{
    float const fPtPosx = static_cast<float>(ptPos.x);
    float const fPtPosy = static_cast<float>(ptPos.y);
    addRect2Buffer
    ( 
        fPtPosx - fPixSize, 
        fPtPosy - fPixSize, 
        fPtPosx + fPixSize, 
        fPtPosy + fPixSize, 
        dwColor 
    );
}

void D3dBuffer::AddBackgroundRect( PixelPoint const & ptPos, DWORD const dwColor, float const fPixSize )
{
    if ( m_bStripMode )
    {
        m_VertexBuffer.AddVertex( static_cast<float>(ptPos.x + fPixSize/2), static_cast<float>(ptPos.y + fPixSize/2), dwColor );
    }
    else
    {
        AddRect( ptPos, dwColor, fPixSize );
    }
}

void D3dBuffer::prepareTranspMode( )
{
    m_d3d_device->GetRenderState( D3DRS_ALPHABLENDENABLE, &m_dwAlphaBlendable );
    m_d3d_device->GetRenderState( D3DRS_SRCBLEND, &m_dwSrcBlend );
    m_d3d_device->GetRenderState( D3DRS_DESTBLEND, &m_dwDstBlend );    

    m_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_d3d_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_d3d_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );    

    m_RectBuffer.ResetVertexBuffer();
}

void D3dBuffer::finishTranspMode( )
{
    RenderRects( );

    m_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, m_dwAlphaBlendable );
    m_d3d_device->SetRenderState( D3DRS_SRCBLEND, m_dwSrcBlend );
    m_d3d_device->SetRenderState( D3DRS_DESTBLEND, m_dwDstBlend );    
}

void D3dBuffer::RenderTranspRect( PixelRect const &rectTransparent, DWORD const dwColor )
{
    assert( m_d3d_device != nullptr );
    prepareTranspMode( );
    addRect2Buffer
    ( 
        static_cast<float>(rectTransparent.left), 
        static_cast<float>(rectTransparent.top), 
        static_cast<float>(rectTransparent.right), 
        static_cast<float>(rectTransparent.bottom), 
        dwColor 
    );
    finishTranspMode( );
}

void D3dBuffer::RenderVertices( )
{
    if ( m_bStripMode )
        renderTriangleStrip( m_VertexBuffer, m_d3d->GetBgIndexBufferStripMode() );
    else
        RenderRects( );
}

void D3dBuffer::renderTriangleStrip( VertexBuffer const & vBuffer, D3dIndexBuffer const * const iBuf ) const
{
    assert( vBuffer.GetNrOfVertices() > 0 );
    assert( m_d3d_device != nullptr );

    vBuffer.LoadVertices( m_d3d_vertexBuffer, m_d3d_device );
    ULONG const ulMaxNrOfPrimitives = iBuf->SetIndices( m_d3d_device );

    HRESULT const hres = m_d3d_device->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, vBuffer.GetNrOfVertices( ), 0, ulMaxNrOfPrimitives );
    assert(hres == D3D_OK);
}

void D3dBuffer::RenderRects( )
{
    if ( m_RectBuffer.GetNrOfVertices() > 0 )
    {
        m_RectBuffer.LoadVertices( m_d3d_vertexBuffer, m_d3d_device );
        D3dIndexBuffer const * const iBuf = m_d3d->GetIndsIndexBuffer();
        iBuf->SetIndices( m_d3d_device );
        HRESULT const hres = m_d3d_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_RectBuffer.GetNrOfVertices(), 0, m_RectBuffer.GetNrOfVertices() / 2 ); 
        assert( hres == D3D_OK ); 
        m_RectBuffer.ResetVertexBuffer();
    }
}

// Finish rendering; page flip.

void D3dBuffer::EndFrame( )
{
    HRESULT hres;
    hres = m_d3d_device->EndScene();                                          assert(hres == D3D_OK);
    hres = m_d3d_swapChain->Present( nullptr, nullptr, m_hWnd, nullptr, 0 );  assert(hres == D3D_OK);
    //lint -esym( 613, D3dBuffer::m_id3dx_font )  possible use of null pointer
    m_id3dx_font->Release();      
    //lint +esym( 613, D3dBuffer::m_id3dx_font ) 
}
