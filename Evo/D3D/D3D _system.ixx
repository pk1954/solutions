// D3dSystem.ixx
//
// D3D

module;

#include <d3dx9core.h>

export module D3dSystem;

import D3dIndexBuffer;

export class D3dSystem
{
public:
	D3dSystem();
	virtual ~D3dSystem();

	IDirect3DDevice9* Create_D3D_Device(HWND const, ULONG const, ULONG const, bool const);

	HRESULT            ResizeD3dSystem(HWND const);

	D3dIndexBuffer const* GetBgIndexBufferStripMode() const { return m_d3d_pIndexBufBgStripMode; };
	D3dIndexBuffer const* GetIndsIndexBuffer()        const { return m_d3d_pIndexBufIndividuals; };
	D3dIndexBuffer const* GetRectIndexBuffer()        const { return m_d3d_pIndexBufRect; };

	LPDIRECT3DVERTEXBUFFER9 CreateVertexBuffer();

	ULONG GetTrianglesPerPrimitive() { return m_ulTrianglesPerPrimitive; }
	ULONG GetVerticesPerPrimitive() { return m_ulVerticesPerPrimitive; }
	ULONG GetMaxNrOfPrimitives() { return m_ulMaxNrOfPrimitives; }
	ULONG GetNrOfVertices() { return m_ulNrOfVertices; }

	bool IsHexagonMode() { return m_bHexagon; }

private:
	bool m_bHexagon;

	ULONG m_ulTrianglesPerPrimitive;
	ULONG m_ulVerticesPerPrimitive;
	ULONG m_ulMaxNrOfPrimitives;
	ULONG m_ulNrOfVertices;

	D3dIndexBuffer* prepareIndices(ULONG const* const, ULONG const);
	D3dIndexBuffer* createStripIndices(ULONG const, ULONG const);
	D3dIndexBuffer* createIndsIndices(ULONG const, ULONG const, bool const);
	D3dIndexBuffer* createRectIndices();

	void rectangleIndices(ULONG* const, ULONG const);
	void hexagonIndices(ULONG* const, ULONG const);

	IDirect3D9* m_d3d_object;
	IDirect3DDevice9* m_d3d_device;

	D3DPRESENT_PARAMETERS m_d3d_presentationParameters;
	D3DMATRIX             m_d3d_matrix;
	D3dIndexBuffer* m_d3d_pIndexBufBgStripMode;
	D3dIndexBuffer* m_d3d_pIndexBufBgNonStripMode;
	D3dIndexBuffer* m_d3d_pIndexBufIndividuals;
	D3dIndexBuffer* m_d3d_pIndexBufRect;    // index buffer for one rectangle 
};
