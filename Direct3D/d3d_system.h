// d3d_system.h : 
//

#pragma once

#include <D3dx9core.h>
#include "d3d_indexBuffer.h"

class D3dSystem
{
public:
    virtual ~D3dSystem( );

    static void Create_D3D_Device( HWND const, ULONG const, ULONG const, BOOL const );
    static D3dSystem * GetSystem( void );

    void               ResetD3dSystem( HWND const );
    void               SetTransform  ( HWND const );
    IDirect3DDevice9 * GetDevice     ( void );

    D3dIndexBuffer const * GetBgIndexBufferStripMode( ) const { return m_d3d_pIndexBufBgStripMode; };
    D3dIndexBuffer const * GetIndsIndexBuffer( )        const { return m_d3d_pIndexBufIndividuals; };
    D3dIndexBuffer const * GetRectIndexBuffer( )        const { return m_d3d_pIndexBufRect; };

	static BOOL GetHexagonMode( ) { return m_bHexagon; };

private:
    void createDevice( HWND const, ULONG const, ULONG const );

    D3dIndexBuffer * prepareIndices( ULONG const * const, ULONG const );
    D3dIndexBuffer * createStripIndices( ULONG const, ULONG const );
    D3dIndexBuffer * createIndsIndices ( ULONG const, ULONG const );
	D3dIndexBuffer * createRectIndices();

	void rectangleIndices( ULONG * const, ULONG const );
	void hexagonIndices  ( ULONG * const, ULONG const );

    IDirect3D9       * m_d3d_object;
    IDirect3DDevice9 * m_d3d_device;

    D3DPRESENT_PARAMETERS m_d3d_presentationParameters;
    D3DMATRIX             m_d3d_matrix;
    D3dIndexBuffer      * m_d3d_pIndexBufBgStripMode; 
    D3dIndexBuffer      * m_d3d_pIndexBufBgNonStripMode; 
    D3dIndexBuffer      * m_d3d_pIndexBufIndividuals; 
    D3dIndexBuffer      * m_d3d_pIndexBufRect;    // index buffer for one rectangle 

    INT  m_iNrSwapChainsInUse;

	static BOOL m_bHexagon;
    static D3dSystem _d3d_;
};
