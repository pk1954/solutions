// D3D_driver.h : 
//

#pragma once

#include <string.h>
#include <d3d9.h>
#include "d3dx9core.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_graphicsInterface.h"
#include "d3d_system.h"
#include "d3d_vertexBuffer.h"

class D3D_driver: public GraphicsInterface
{
public:

    D3D_driver( ULONG const );
    ~D3D_driver();

    virtual bool      StartFrame( HWND const, HDC const  );
    virtual void      SetFontSize( PIXEL const );
    virtual void      AddIndividual( PixelPoint const, COLORREF const, float const );    
    virtual void      AddBackGround( PixelPoint const, COLORREF const, float const );    
    virtual void      RenderIndividuals( );
    virtual void      RenderBackground( );
    virtual void      RenderTranspRect( PixelRect const &, unsigned int const, COLORREF const );
    virtual void      DisplayGraphicsText( PixelRect const &, std::wstring const & );
    virtual PixelRect CalcGraphicsRect( std::wstring const & );
	virtual void      EndFrame( );
	 
    virtual void SetStripMode( tBoolOp const bOp ) 
	{ 
		ApplyOp( m_bStripMode, bOp ); 
	};

    virtual bool GetStripMode( ) 
	{ 
		return m_bStripMode; 
	};

private:
    
    ULONG m_ulTrianglesPerPrimitive;
	ULONG m_ulVerticesPerPrimitive;
	ULONG m_ulMaxNrOfPrimitives;
    ULONG m_ulNrOfVertices;

    VertexBuffer * m_pVertBufStripMode;
    VertexBuffer * m_pVertBufPrimitives;

    HWND                    m_hwnd;
	D3dSystem             * m_d3d;
    IDirect3DDevice9      * m_d3d_device;
    LPDIRECT3DVERTEXBUFFER9 m_d3d_vertexBuffer; 
    LPDIRECT3DSWAPCHAIN9    m_d3d_swapChain;
    ID3DXFont             * m_id3dx_font;
    bool                    m_bStripMode;
    DWORD                   m_dwAlphaBlendable;
    DWORD                   m_dwSrcBlend;
    DWORD                   m_dwDstBlend;

    static D3DXFONT_DESC    m_d3dx_font_desc;  // identical for all buffers, even on systems with multiple displays

    bool setFont( );
    void addRectangle( float const, float const, D3DCOLOR const, float const );    
    void addHexagon  ( float const, float const, D3DCOLOR const, float const, float const );    
    void addRect2Buffer( float const, float const, float const, float const, D3DCOLOR const );
    void renderTriangleStrip( ) const;
    void renderPrimitives( D3dIndexBuffer const * const );
    void prepareTranspMode( );
    void finishTranspMode( );

	D3DCOLOR COLORREFtoD3DCOLOR( unsigned int const, COLORREF const );
};
