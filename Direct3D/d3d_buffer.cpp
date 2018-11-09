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

D3dBuffer::D3dBuffer( ULONG const ulNrOfPoints ) 
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
}

void D3dBuffer::D3D_DrawText( PixelRect const & pixRect, wstring const & wstr, D3DCOLOR col )
{
//	long const lClientWinHeight = Util::GetClientWindowHeight( m_hwnd );
	RECT rect 
	{                                         
		pixRect.m_lLeft,					  
		pixRect.m_lTop, 
		pixRect.m_lRight,                    
		pixRect.m_lBottom	   
	};
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

BOOL D3dBuffer::StartFrame( HWND hwnd )
{
    HRESULT hres;

	m_hwnd = hwnd;

    hres = m_d3d->ResetD3dSystem( hwnd );                                          if ( hres != D3D_OK ) return FALSE;
    hres = m_d3d_device->SetRenderState( D3DRS_LIGHTING, FALSE );                  if ( hres != D3D_OK ) return FALSE;
    hres = m_d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );           if ( hres != D3D_OK ) return FALSE;
    hres = m_d3d_device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );                    if ( hres != D3D_OK ) return FALSE;
    hres = m_d3d_device->Clear( 0, nullptr, D3DCLEAR_TARGET, CLR_WHITE, 1.0f, 0 ); if ( hres != D3D_OK ) return FALSE;
    hres = m_d3d_device->BeginScene( );                                            if ( hres != D3D_OK ) return FALSE;
    if ( m_bStripMode )
        m_pVertBufStripMode->ResetVertexBuffer();
    m_pVertBufPrimitives->ResetVertexBuffer();
    if ( ! setFont( ) )
		return FALSE;
	return TRUE;
}

BOOL D3dBuffer::setFont( )
{
    HRESULT const hres = D3DXCreateFontIndirect( m_d3d_device, &m_d3dx_font_desc, &m_id3dx_font );   
    if ( hres != D3D_OK )          return FALSE;
    if ( m_id3dx_font == nullptr ) return FALSE;
	return TRUE;
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

void D3dBuffer::AddIndividualPrimitive( PixelPoint const ptPos, DWORD const dwColor, float const fPixSize )
{
	static float const SQRT3 = static_cast<float>( sqrt( 3 ) );

    float const fPtPosx = static_cast<float>( ptPos.x );
    float const fPtPosy = static_cast<float>( ptPos.y );

    if ( m_bStripMode )
    {
		addRectangle( fPtPosx, fPtPosy, dwColor, fPixSize );
	}
	else
	{
		if ( m_d3d->GetHexagonMode( ) )
			addHexagon( fPtPosx, fPtPosy, dwColor, fPixSize, SQRT3 * fPixSize );
		else
			addRectangle( fPtPosx, fPtPosy, dwColor, fPixSize );
	}
}

void D3dBuffer::AddBackgroundPrimitive( PixelPoint const ptPos, DWORD const dwColor, float const fPixSize )
{
	static float const INVERSE_SQRT3 = static_cast<float>( 1 / sqrt( 3 ) );

	float const fPtPosx = static_cast<float>( ptPos.x );
    float const fPtPosy = static_cast<float>( ptPos.y );

	float const fPixSizeHalf = fPixSize / 2;

    if ( m_bStripMode )
    {
        m_pVertBufStripMode->AddVertex( fPtPosx, fPtPosy, dwColor );
    }
    else
    {
		if ( m_d3d->GetHexagonMode( ) )
			addHexagon( fPtPosx, fPtPosy, dwColor, fPixSize * INVERSE_SQRT3, fPixSize );
		else
			addRectangle( fPtPosx, fPtPosy, dwColor, fPixSizeHalf );
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
	if ( rectTransparent.IsNotEmpty() )
	{
		assert( m_d3d_device != nullptr );
		prepareTranspMode( );

		addRect2Buffer
		( 
			static_cast<float>( rectTransparent.m_lLeft   ), 
			static_cast<float>( rectTransparent.m_lTop    ), 
			static_cast<float>( rectTransparent.m_lRight  ), 
			static_cast<float>( rectTransparent.m_lBottom ), 
			dwColor 
		);

		renderPrimitives( m_d3d->GetRectIndexBuffer() );

		finishTranspMode( );
	}
}

void D3dBuffer::RenderBackground( )
{
    if ( m_bStripMode )
        renderTriangleStrip( );
    else
        renderPrimitives( m_d3d->GetIndsIndexBuffer() );
}

void D3dBuffer::RenderIndividuals( )
{
    renderPrimitives( m_d3d->GetIndsIndexBuffer() );
}

void D3dBuffer::renderTriangleStrip( ) const
{
	HRESULT hres;

    assert( m_pVertBufStripMode->GetNrOfVertices() > 0 );
    assert( m_d3d_device != nullptr );

    D3dIndexBuffer const * const iBuf = m_d3d->GetBgIndexBufferStripMode();
    
    hres = m_pVertBufStripMode->LoadVertices( m_d3d_vertexBuffer, m_d3d_device );

	iBuf->SetIndices( m_d3d_device );
    
	ULONG ulNrOfVertices   = m_pVertBufStripMode->GetNrOfVertices( );
	UINT  uiNrOfPrimitives = ulNrOfVertices - 2;
	
	hres = m_d3d_device->DrawIndexedPrimitive
	( 
		D3DPT_TRIANGLESTRIP, 
		0, 
		0, 
		m_pVertBufStripMode->GetNrOfVertices( ), 
		0, 
		iBuf->GetMaxNrOfPrimitives( ) 
	);

    assert( hres == D3D_OK );
}

void D3dBuffer::renderPrimitives( D3dIndexBuffer const * const iBuf )
{
	ULONG const ulNrOfPrimitives = m_ulTrianglesPerPrimitive * m_pVertBufPrimitives->GetNrOfVertices() / m_ulVerticesPerPrimitive;
	HRESULT hres;
	
	if ( ulNrOfPrimitives > 0 )
    {
        hres = m_pVertBufPrimitives->LoadVertices( m_d3d_vertexBuffer, m_d3d_device );
        assert( hres == D3D_OK ); 

        iBuf->SetIndices( m_d3d_device );
        hres = m_d3d_device->DrawIndexedPrimitive
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
    hres = m_d3d_swapChain->Present( nullptr, nullptr, m_hwnd, nullptr, 0 );  assert(hres == D3D_OK);
    //lint -esym( 613, D3dBuffer::m_id3dx_font )  possible use of null pointer
    m_id3dx_font->Release();      
    //lint +esym( 613, D3dBuffer::m_id3dx_font ) 
}
