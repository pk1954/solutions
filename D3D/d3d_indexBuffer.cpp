// d3d_indexBuffer.cpp
//

#include "stdafx.h"
#include <assert.h>
#include <d3d9.h>
#include "d3d_indexBuffer.h"

D3dIndexBuffer::D3dIndexBuffer( LPDIRECT3DINDEXBUFFER9 const lpD3DiBuf, ULONG const ulNrOfIndices ) :
    m_d3d_indexBuffer( lpD3DiBuf ),
    m_ulNrOfIndices( ulNrOfIndices )
{
}

void D3dIndexBuffer::SetIndices( IDirect3DDevice9 * const device ) const  // returns maximum number of primitives described by indices
{
    HRESULT const hres = device->SetIndices( m_d3d_indexBuffer );  
    assert( hres == D3D_OK );
}

void D3dIndexBuffer::Release() 
{ 
    m_d3d_indexBuffer->Release(); 
};
