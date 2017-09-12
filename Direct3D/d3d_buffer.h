// d3d_buffer.h : 
//

#pragma once

#include <string.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include "pixelPoint.h"
#include "win32_util.h"
#include "d3d_system.h"
#include "d3d_vertexBuffer.h"

enum class tTesselation
{
	RECTANGLE,
	HEXAGON
};

class D3dBuffer
{
public:

    D3dBuffer( HWND const, ULONG const, tTesselation const );
    ~D3dBuffer();

    void StartFrame( );
    void ResetFont( int const );
    void AddIndividualPrimitive( PixelPoint const &, DWORD const, float const );    
    void AddBackgroundPrimitive( PixelPoint const &, DWORD const, float const );    
    void RenderIndividuals( );
    void RenderBackground( );
    void RenderTranspRect( PixelRect const &, DWORD const );
    void D3D_DrawText( PixelRect, wstring const &, D3DCOLOR );
    void EndFrame( );

    void SetStripMode( tBoolOp const bOp ) { Util::ApplyOp( m_bStripMode, bOp ); };

private:
    
    HWND m_hWnd;

	tTesselation m_tesselationMode;

    ULONG const m_ulTrianglesPerPrimitive;
	ULONG const m_ulVerticesPerPrimitive;
	ULONG const m_ulMaxNrOfPrimitives;
    ULONG const m_ulNrOfVertices;

    VertexBuffer m_VertBufStripMode;
    VertexBuffer m_VertBufPrimitives;

    D3dSystem             * m_d3d;
    IDirect3DDevice9      * m_d3d_device;
    LPDIRECT3DVERTEXBUFFER9 m_d3d_vertexBuffer; 
    LPDIRECT3DSWAPCHAIN9    m_d3d_swapChain;
    ID3DXFont             * m_id3dx_font;
    BOOL                    m_bStripMode;
    DWORD                   m_dwAlphaBlendable;
    DWORD                   m_dwSrcBlend;
    DWORD                   m_dwDstBlend;

    static D3DXFONT_DESC    m_d3dx_font_desc;  // identical for all buffers, even on systems with multiple displays

    void setFont( );
    void addRectangle( float const, float const, DWORD const, float const );    
    void addHexagon  ( float const, float const, DWORD const, float const, float const );    
    void addRect2Buffer( float const, float const, float const, float const, DWORD const );
    void renderTriangleStrip( ) const;
    void renderPrimitives( );
    void prepareTranspMode( );
    void finishTranspMode( );
};
