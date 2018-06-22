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
    FIXED_PITCH|FF_MODERN,     //  PitchAndFamily
    L""                        //  FaceName
};

D3dBuffer::D3dBuffer
( 
	HWND  const hWnd, 
	ULONG const ulNrOfPoints
) 
{
    HRESULT hres;

    m_d3d = D3dSystem::GetSystem( );

	m_ulTrianglesPerPrimitive = m_d3d->GetHexagonMode( ) ? 4 : 2; // Hexagon is made of 4 triangles, rect of 2 triangles
	m_ulVerticesPerPrimitive  = m_d3d->GetHexagonMode( ) ? 6 : 4; // Hexagon has 6 vertices, rect has 4
    m_ulMaxNrOfPrimitives     = ulNrOfPoints;
    m_ulNrOfVertices          = m_ulMaxNrOfPrimitives * m_ulVerticesPerPrimitive;
    m_pVertBufStripMode       = new VertexBuffer( ulNrOfPoints );
    m_pVertBufPrimitives      = new VertexBuffer( m_ulNrOfVertices );

    m_d3d_device = m_d3d->GetDevice( );                                  assert( m_d3d_device != nullptr );
    hres = m_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );        assert( hres == D3D_OK );
    hres = m_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ); assert( hres == D3D_OK );
    hres = m_d3d_device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );          assert( hres == D3D_OK );

    hres = m_d3d_device->CreateVertexBuffer
    (
        sizeof( Vertex ) * m_ulNrOfVertices,
        0,  //D3DUSAGE_DYNAMIC,
        D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
        D3DPOOL_MANAGED,   //D3DPOOL_DEFAULT,
        & m_d3d_vertexBuffer,
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
    ResetFont( 9 );
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

void D3dBuffer::D3D_DrawText( PixelRect pixRect, wstring const & wstr, D3DCOLOR col )
{
    assert( m_id3dx_font != nullptr );
    //lint -esym( 613, D3dBuffer::m_id3dx_font )  possible use of null pointer
	RECT rect = Util::PixelRect2RECT( pixRect );
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
        m_pVertBufStripMode->ResetVertexBuffer();

    m_pVertBufPrimitives->ResetVertexBuffer();

    setFont( );   
}

void D3dBuffer::setFont( )
{
    HRESULT const hres = D3DXCreateFontIndirect( m_d3d_device, &m_d3dx_font_desc, &m_id3dx_font );   
    assert( hres == D3D_OK );
    assert( m_id3dx_font != nullptr );
}

void D3dBuffer::ResetFont( int const nPointSize )
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

    HDC const hDC = GetDC( nullptr );
    int const iLogPixels = GetDeviceCaps( hDC, LOGPIXELSY );
    ReleaseDC( nullptr, hDC );
    m_d3dx_font_desc.Height = -( MulDiv( nPointSize, iLogPixels, 72 ) );
	m_d3dx_font_desc.Width  = m_d3dx_font_desc.Height / 2;
    setFont( );   
}

void D3dBuffer::addRect2Buffer( float const fWest, float const fNorth, float const fEast, float const fSouth, DWORD const dwColor )
{    
    m_pVertBufPrimitives->AddVertex( fWest, fNorth, dwColor );
    m_pVertBufPrimitives->AddVertex( fEast, fNorth, dwColor );
    m_pVertBufPrimitives->AddVertex( fEast, fSouth, dwColor );
    m_pVertBufPrimitives->AddVertex( fWest, fSouth, dwColor );
}

void D3dBuffer::addRectangle( float const fPtPosx, float const fPtPosy, DWORD const dwColor, float const fPixSize )
{
    addRect2Buffer
    ( 
        fPtPosx - fPixSize, 
        fPtPosy - fPixSize, 
        fPtPosx + fPixSize, 
        fPtPosy + fPixSize, 
        dwColor 
    );
}

void D3dBuffer::addHexagon( float const fPtPosx, float const fPtPosy, DWORD const dwColor, float const fPixSizeX, float const fPixSizeY )
{
	m_pVertBufPrimitives->AddVertex( fPtPosx - fPixSizeX     - 0.5f, fPtPosy,                 dwColor );     // west
	m_pVertBufPrimitives->AddVertex( fPtPosx - fPixSizeX / 2 - 0.5f, fPtPosy - fPixSizeY / 2, dwColor );     // north west
	m_pVertBufPrimitives->AddVertex( fPtPosx - fPixSizeX / 2 - 0.5f, fPtPosy + fPixSizeY / 2, dwColor );     // south west
	m_pVertBufPrimitives->AddVertex( fPtPosx + fPixSizeX / 2 + 0.5f, fPtPosy - fPixSizeY / 2, dwColor );     // north east
 	m_pVertBufPrimitives->AddVertex( fPtPosx + fPixSizeX / 2 + 0.5f, fPtPosy + fPixSizeY / 2, dwColor );     // south east
	m_pVertBufPrimitives->AddVertex( fPtPosx + fPixSizeX     + 0.5f, fPtPosy,                 dwColor );     // east
}

void D3dBuffer::AddIndividualPrimitive( PixelPoint const & ptPos, DWORD const dwColor, float const fPixSize )
{
	static float const SQRT3 = static_cast<float>( sqrt( 3 ) );

    float const fPtPosx = static_cast<float>( ptPos.x );
    float const fPtPosy = static_cast<float>( ptPos.y );

	if ( m_d3d->GetHexagonMode( ) )
		addHexagon( fPtPosx, fPtPosy, dwColor, fPixSize, SQRT3 * fPixSize );
	else
		addRectangle( fPtPosx, fPtPosy, dwColor, fPixSize );
}

void D3dBuffer::AddBackgroundPrimitive( PixelPoint const & ptPos, DWORD const dwColor, float const fPixSize )
{
	static float const INVERSE_SQRT3 = static_cast<float>( 1 / sqrt( 3 ) );

	float const fPtPosx      = static_cast<float>( ptPos.x );
    float const fPtPosy      = static_cast<float>( ptPos.y );
	float const fPixSizeHalf = fPixSize / 2;

    if ( m_bStripMode )
    {
        m_pVertBufStripMode->AddVertex( fPtPosx, fPtPosy, dwColor );
    }
    else
    {
		if ( m_d3d->GetHexagonMode( ) )
		{
			addHexagon( fPtPosx, fPtPosy, dwColor, fPixSize * INVERSE_SQRT3, fPixSize );
		}
		else
		{
			addRectangle( fPtPosx, fPtPosy, dwColor, fPixSize );
		}
    }
}

void D3dBuffer::prepareTranspMode( )
{
    m_d3d_device->GetRenderState( D3DRS_ALPHABLENDENABLE, & m_dwAlphaBlendable );
    m_d3d_device->GetRenderState( D3DRS_SRCBLEND,         & m_dwSrcBlend );
    m_d3d_device->GetRenderState( D3DRS_DESTBLEND,        & m_dwDstBlend );    

    m_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_d3d_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_d3d_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );    

    m_pVertBufPrimitives->ResetVertexBuffer();
}

void D3dBuffer::finishTranspMode( )
{
    m_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, m_dwAlphaBlendable );
    m_d3d_device->SetRenderState( D3DRS_SRCBLEND, m_dwSrcBlend );
    m_d3d_device->SetRenderState( D3DRS_DESTBLEND, m_dwDstBlend );    
}

void D3dBuffer::RenderTranspRect( PixelRect const & rectTransparent, DWORD const dwColor )
{
    assert( m_d3d_device != nullptr );
    prepareTranspMode( );

	addRect2Buffer
    ( 
        static_cast<float>( rectTransparent.left   ), 
        static_cast<float>( rectTransparent.top    ), 
        static_cast<float>( rectTransparent.right  ), 
        static_cast<float>( rectTransparent.bottom ), 
        dwColor 
    );

	renderPrimitives( );
    finishTranspMode( );
}

void D3dBuffer::RenderBackground( )
{
    if ( m_bStripMode )
        renderTriangleStrip( );
    else
        renderPrimitives( );
}

void D3dBuffer::RenderIndividuals( )
{
    renderPrimitives( );
}

void D3dBuffer::renderTriangleStrip( ) const
{
    assert( m_pVertBufStripMode->GetNrOfVertices() > 0 );
    assert( m_d3d_device != nullptr );

    m_pVertBufStripMode->LoadVertices( m_d3d_vertexBuffer, m_d3d_device );

    D3dIndexBuffer const * const iBuf                = m_d3d->GetBgIndexBufferStripMode();
    ULONG          const         ulMaxNrOfPrimitives = iBuf->SetIndices( m_d3d_device );
    HRESULT        const         hres                = m_d3d_device->DrawIndexedPrimitive
	( 
		D3DPT_TRIANGLESTRIP, 
		0, 
		0, 
		m_pVertBufStripMode->GetNrOfVertices( ), 
		0, 
		ulMaxNrOfPrimitives 
	);

    assert( hres == D3D_OK );
}

void D3dBuffer::renderPrimitives( )
{
	ULONG const ulNrOfPrimitives = m_ulTrianglesPerPrimitive * m_pVertBufPrimitives->GetNrOfVertices() / m_ulVerticesPerPrimitive;
	
	if ( ulNrOfPrimitives > 0 )
    {
        m_pVertBufPrimitives->LoadVertices( m_d3d_vertexBuffer, m_d3d_device );
        D3dIndexBuffer const * const iBuf                = m_d3d->GetIndsIndexBuffer();
        ULONG          const         ulMaxNrOfPrimitives = iBuf->SetIndices( m_d3d_device );
        HRESULT        const         hres                = m_d3d_device->DrawIndexedPrimitive
		( 
			D3DPT_TRIANGLELIST, 
			0, 
			0, 
			m_pVertBufPrimitives->GetNrOfVertices(), 
			0, 
			ulNrOfPrimitives 
		); 
        assert( hres == D3D_OK ); 
        m_pVertBufPrimitives->ResetVertexBuffer();
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
