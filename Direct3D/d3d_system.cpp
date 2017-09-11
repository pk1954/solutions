// d3d_buffer.cpp
//

#include "stdafx.h"
#include <d3d9.h>
#include <D3dx9core.h>
#include "win32_util.h"
#include "d3d_system.h"

//lint -esym( 613, D3dSystem::m_d3d_device )   possible use of null pointer

D3dSystem D3dSystem::_d3d_;

D3dSystem * D3dSystem::GetSystem( void )
{
	return & D3dSystem::_d3d_;
}

void D3dSystem::Create( HWND const hWndApp, ULONG const ulWidth, ULONG const ulHeight )
{
	_d3d_.m_d3d_device         = nullptr;
	_d3d_.m_d3d_object         = Direct3DCreate9(D3D_SDK_VERSION); assert( _d3d_.m_d3d_object != nullptr );
	_d3d_.m_iNrSwapChainsInUse = 0;
	_d3d_.createDevice( hWndApp, ulWidth, ulHeight );
}

IDirect3DDevice9 * D3dSystem::GetDevice( void )
{
	assert( m_d3d_device != nullptr );
	return m_d3d_device;
}

void D3dSystem::ResetD3dSystem( HWND const hWnd )
{
	PixelRectSize const pntSize = Util::GetClRectSize( hWnd );

	m_d3d_presentationParameters.hDeviceWindow    = hWnd;
	m_d3d_presentationParameters.BackBufferWidth  = static_cast<unsigned int>( pntSize.GetWidth ( ) );
	m_d3d_presentationParameters.BackBufferHeight = static_cast<unsigned int>( pntSize.GetHeight( ) );

	assert( m_d3d_device != nullptr );
	HRESULT const hres = m_d3d_device->Reset( & m_d3d_presentationParameters ); 
	assert( hres == D3D_OK) ;
}

void D3dSystem::SetTransform( HWND const hWnd )
{
	PixelRectSize const pntSize = Util::GetClRectSize( hWnd );

	m_d3d_matrix._11 = 2.0f / static_cast<float>( pntSize.GetWidth() );
	m_d3d_matrix._22 = 2.0f / static_cast<float>( pntSize.GetHeight() );

	assert( m_d3d_device != nullptr );
	HRESULT const hres = m_d3d_device->SetTransform( D3DTS_PROJECTION, &m_d3d_matrix );  assert(hres == D3D_OK);
}

void D3dSystem::createDevice( HWND const hWnd, ULONG const ulModelWidth, ULONG const ulModelHeight )
{
	PixelRectSize const pntSize = Util::GetClRectSize( hWnd );

	ZeroMemory( & m_d3d_presentationParameters, sizeof(D3DPRESENT_PARAMETERS) );
	m_d3d_presentationParameters.Windowed               = TRUE;
	m_d3d_presentationParameters.MultiSampleType        = D3DMULTISAMPLE_NONE;
	m_d3d_presentationParameters.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_d3d_presentationParameters.EnableAutoDepthStencil = FALSE;
	m_d3d_presentationParameters.BackBufferFormat       = D3DFMT_X8R8G8B8;
	m_d3d_presentationParameters.hDeviceWindow          = hWnd;
	m_d3d_presentationParameters.Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_d3d_presentationParameters.BackBufferCount        = 1; // D3DPRESENT_BACK_BUFFERS_MAX;
	m_d3d_presentationParameters.BackBufferWidth        = static_cast<unsigned int>( pntSize.GetWidth ( ) );
	m_d3d_presentationParameters.BackBufferHeight       = static_cast<unsigned int>( pntSize.GetHeight( ) );
	m_d3d_presentationParameters.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT const hres = m_d3d_object->CreateDevice
	(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		hWnd,
		D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		& m_d3d_presentationParameters, 
		& m_d3d_device
	);

	assert( hres == D3D_OK );
	
	//lint -e708     union initialization
	D3DMATRIX const m = 
	{
		 1.0f,   0.0f,  0.0f,  0.0f,
		 0.0f,   1.0f,  0.0f,  0.0f,
		 0.0f,   0.0f,  1.0f,  0.0f,
		-1.0f,  -1.0f,  0.0f,  1.0f
	};
	//lint +e708

	m_d3d_matrix = m;
	m_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_d3d_pIndexBufBgStripMode    = createStripIndices( ulModelWidth, ulModelHeight );    // Index buffer for food
	m_d3d_pIndexBufBgNonStripMode = createIndsIndices ( ulModelWidth, ulModelHeight );    // Index buffer for food
	m_d3d_pIndexBufIndividuals    = createIndsIndices ( ulModelWidth, ulModelHeight );    // Index buffer for individuals
}

D3dSystem::~D3dSystem( )
{
	try
	{
		m_d3d_device->Release();
		m_d3d_pIndexBufBgStripMode   ->Release();     // close and release the food index buffer
		m_d3d_pIndexBufBgNonStripMode->Release();     // close and release the food index buffer
		m_d3d_pIndexBufIndividuals   ->Release();     // close and release the individuals index buffer
	}
	catch ( ... )
	{
		exit( 1 );
	};

	delete m_d3d_pIndexBufBgStripMode;
	delete m_d3d_pIndexBufBgNonStripMode;
	delete m_d3d_pIndexBufIndividuals;
	m_d3d_device = nullptr;
	m_d3d_object = nullptr;
}

D3dIndexBuffer * D3dSystem::prepareIndices( ULONG const * const pulIndex, ULONG const ulNrOfIndices )       // lock m_d3d_indexBuffer and load the indices into it
{
	LPDIRECT3DINDEXBUFFER9 d3d_indexBuffer;

	assert( m_d3d_device != nullptr );
	HRESULT hres = m_d3d_device->CreateIndexBuffer
	(
		sizeof( ULONG ) * ulNrOfIndices,
		0, 
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		& d3d_indexBuffer,
		nullptr
	);

	assert( hres == D3D_OK );

	VOID *pVoid;
	hres = d3d_indexBuffer->Lock( 0, 0, static_cast<void**>(&pVoid), 0 );  assert(hres == D3D_OK);
	memcpy( pVoid, pulIndex, ulNrOfIndices * sizeof(ULONG) );
	hres = d3d_indexBuffer->Unlock( );                        assert(hres == D3D_OK);

	return new D3dIndexBuffer( d3d_indexBuffer, ulNrOfIndices );
}

D3dIndexBuffer * D3dSystem::createIndsIndices( ULONG const ulModelWidth, ULONG const ulModelHeight )
{
	bool    const HEXAGON = TRUE;
	ULONG   const ulVerticesPerPrimitive  = HEXAGON ? 6 : 4; // Hexagon has 6 vertices, rectangle has 4
	ULONG   const ulTrianglesPerPrimitive = HEXAGON ? 4 : 2; // Hexagon is made of 4 triangles, rect of 2 triangles
	ULONG   const ulNrOfPrimitives        = ulModelWidth * ulModelHeight;
	ULONG   const ulNrOfIndices           = 3 * ulTrianglesPerPrimitive * ulNrOfPrimitives;
	ULONG * const pulIndices              = new ULONG[ulNrOfIndices];
	ULONG *       pulIndexRun             = pulIndices;
	ULONG   const ulStop                  = ulNrOfPrimitives * ulVerticesPerPrimitive;

	for ( ULONG ulBase = 0; ulBase < ulStop; ulBase += ulVerticesPerPrimitive ) 
	if ( HEXAGON )
	{
		* pulIndexRun++ = ulBase + 0;
		* pulIndexRun++ = ulBase + 1;
		* pulIndexRun++ = ulBase + 3;

		* pulIndexRun++ = ulBase + 3;
		* pulIndexRun++ = ulBase + 5;
		* pulIndexRun++ = ulBase + 4;

		* pulIndexRun++ = ulBase + 4;
		* pulIndexRun++ = ulBase + 2;
		* pulIndexRun++ = ulBase + 0;

		* pulIndexRun++ = ulBase + 0;
		* pulIndexRun++ = ulBase + 3;
		* pulIndexRun++ = ulBase + 4;
	}
	else
	{
		* pulIndexRun++ = ulBase + 0;
		* pulIndexRun++ = ulBase + 1;
		* pulIndexRun++ = ulBase + 3;

		* pulIndexRun++ = ulBase + 1;
		* pulIndexRun++ = ulBase + 2;
		* pulIndexRun++ = ulBase + 3;
	}

	D3dIndexBuffer * const pIndexBuffer = prepareIndices( pulIndices, ulNrOfIndices );

	delete[] pulIndices;

	return pIndexBuffer;
}

D3dIndexBuffer * D3dSystem::createStripIndices( ULONG const ulModelWidth, ULONG const ulModelHeight )
{
	ULONG   const ulNrOfIndices = 2 * ulModelWidth * (ulModelHeight - 1) + (ulModelHeight - 1) - 1;
	ULONG * const pulIndices    = new ULONG[ulNrOfIndices];
	ULONG *       pulIndexRun   = pulIndices;

	for ( ULONG y = 0; y < ulModelHeight - 1; ++y )
	{
		ULONG       x;
		ULONG const ulWidthY = ulModelWidth * y;

		if ( y % 2 == 0 ) // Even rows move left to right
		{
			ULONG const ulStop = ulModelWidth + ulWidthY;
			for ( x = ulWidthY; x < ulStop; ++x )
			{
				*pulIndexRun++ = x;                     
				*pulIndexRun++ = x + ulModelWidth;           
			}

			if ( y != ulModelHeight - 2)     // Insert degenerate vertex if this isn't the last row
			{
				*pulIndexRun++ = x - 1;               
			}
		}
		else                 // Odd rows move right to left
		{
			for ( x = ulWidthY + ulModelWidth - 1; x >= ulWidthY; --x )
			{
				*pulIndexRun++ = x;                      
				*pulIndexRun++ = x + ulModelWidth;             
			}

			if ( y != ulModelHeight - 2)     // Insert degenerate vertex if this isn't the last row
			{
				*pulIndexRun++ = x + 1;            
			}
		}
	}

	assert( pulIndexRun - pulIndices == static_cast<int>(ulNrOfIndices) );

	D3dIndexBuffer * const pRes = prepareIndices( pulIndices, ulNrOfIndices );

	delete[] pulIndices;

	return pRes;
}
