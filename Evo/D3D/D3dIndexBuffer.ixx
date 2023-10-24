// D3dIndexBuffer.ixx
//
// D3D

module;

#include <d3d9.h>

export module D3dIndexBuffer;

export class D3dIndexBuffer
{
public:
    D3dIndexBuffer(LPDIRECT3DINDEXBUFFER9 const, ULONG const);

    void  SetIndices(IDirect3DDevice9* const) const;
    ULONG GetMaxNrOfPrimitives() const { return m_ulNrOfIndices - 2; };	// returns maximum number of primitives described by indices
    void  Release();

private:
    LPDIRECT3DINDEXBUFFER9 m_d3d_indexBuffer;
    ULONG                  m_ulNrOfIndices;
};
