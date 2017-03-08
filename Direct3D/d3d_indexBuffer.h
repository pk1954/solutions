// d3d_indexBuffer.h : 
//

#pragma once

#include <d3d9.h>

class D3dIndexBuffer
{
public:
    D3dIndexBuffer( LPDIRECT3DINDEXBUFFER9 const, ULONG const );

    ULONG SetIndices( IDirect3DDevice9 * const ) const;   // returns maximum number of primitives described by indices
    void  Release(); 

private:
    LPDIRECT3DINDEXBUFFER9 m_d3d_indexBuffer; 
    ULONG                  m_ulNrOfIndices;
};
