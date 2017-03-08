// d3d_vertexBuffer.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <d3d9.h>
#include "d3d_vertexBuffer.h"

VertexBuffer::VertexBuffer( ULONG const ulSize )
    : m_vertexVector()
{
    m_vertexVector.reserve( ulSize );
    ResetVertexBuffer();
}

void VertexBuffer::LoadVertices   // lock m_d3d_vertexBuffer and load the vertices into it
(
    LPDIRECT3DVERTEXBUFFER9 const d3d_vertexBuffer, 
    IDirect3DDevice9      * const d3d_device
) const  
{
    HRESULT hres;
    VOID   *pVoid;

    hres = d3d_vertexBuffer->Lock( 0, 0, static_cast<void**>(&pVoid), 0 );         assert(hres == D3D_OK);
    //lint -e1415   Pointer to non-POD class Vertex function memcpy (arg. no. 2) 
    memcpy( pVoid, m_vertexVector.data(), m_vertexVector.size() * sizeof(Vertex) );
    //lint +e1415
    hres = d3d_vertexBuffer->Unlock( );                                            assert(hres == D3D_OK);
    assert( d3d_device != nullptr );
    hres = d3d_device->SetStreamSource( 0, d3d_vertexBuffer, 0, sizeof(Vertex) );  assert(hres == D3D_OK); // select the vertex buffer to display

}
