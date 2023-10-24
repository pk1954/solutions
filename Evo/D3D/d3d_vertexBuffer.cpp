// d3d_vertexBuffer.cpp
//
// D3D

#include <cassert>
#include <d3d9.h>
#include "d3d_vertexBuffer.h"

VertexBuffer::VertexBuffer(ULONG const ulSize)
    : m_vertexVector()
{
    m_vertexVector.reserve(ulSize);
    ResetVertexBuffer();
}

HRESULT VertexBuffer::LoadVertices   // lock m_d3d_vertexBuffer and load the vertices into it
(
    LPDIRECT3DVERTEXBUFFER9 const d3d_vertexBuffer, 
    IDirect3DDevice9      * const d3d_device
) const  
{
    HRESULT hres;
    VOID   *pVoid;

	size_t bytesToLoad = m_vertexVector.size() * sizeof(Vertex);

    hres = d3d_vertexBuffer->Lock(0, CastToUnsignedInt(bytesToLoad), static_cast<void**>(&pVoid), 0);
	if (hres != D3D_OK)
		return hres;
    
    memcpy(pVoid, m_vertexVector.data(), bytesToLoad);
    
	hres = d3d_vertexBuffer->Unlock();
	if (hres != D3D_OK)
		return hres;

    hres = d3d_device->SetStreamSource(0, d3d_vertexBuffer, 0, sizeof(Vertex)); // select the vertex buffer to display
	return hres;
}
