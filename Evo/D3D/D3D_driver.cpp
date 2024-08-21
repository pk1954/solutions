// D3D_driver.cpp
//

module;

#include <d3d9.h>
#include <d3dx9core.h>

module D3D_driver;

import std;
import Debug;
import Types;
import WinBasics;
import D3D_driver;
import D3dSystem;

using std::wstring;

static COLORREF const CLR_WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);

D3DXFONT_DESC D3D_driver::m_d3dx_font_desc =
{
    0,                         //  Height (will be set during initializition)
    0,                         //  Width
    FW_NORMAL,                 //  Weight
    1,                         //  MipLevels
    false,                     //  Italic
    DEFAULT_CHARSET,           //  CharSet
    OUT_DEFAULT_PRECIS,        //  OutputPrecision
    ANTIALIASED_QUALITY,       //  Quality
    FIXED_PITCH|FF_MODERN,     //  PitchAndFamily
    L""                        //  FaceName
};

D3D_driver::D3D_driver():
	m_fOrtho(fPixelPoint::NULL_VAL()),
	m_d3d(nullptr),
    m_d3d_device(nullptr),
    m_id3dx_font(nullptr),
    m_pVertBufStripMode(nullptr),
	m_pVertBufPrimitives(nullptr)
{
	m_d3d = new D3dSystem();
}

D3D_driver::~D3D_driver()
{
	try
	{
		ULONG const ulres = m_d3d_vertexBuffer->Release();    
		Assert(ulres == 0);
	}
	catch (...)
	{
		exit(1);
	};

	delete m_d3d;
	m_d3d_vertexBuffer = nullptr;
	m_d3d_swapChain    = nullptr;
	m_d3d_device       = nullptr;
	m_id3dx_font       = nullptr;
	m_d3d              = nullptr;
}

void D3D_driver::Initialize
(
	HWND  const hwndApp, 
	ULONG const ulModelWidth, 
	ULONG const ulModelHeight, 
	bool  const bHexagon 
) 
{
	m_d3d_device = m_d3d->Create_D3D_Device(hwndApp, ulModelWidth, ulModelHeight, bHexagon);

	m_pVertBufStripMode  = new VertexBuffer(m_d3d->GetMaxNrOfPrimitives());
    m_pVertBufPrimitives = new VertexBuffer(m_d3d->GetNrOfVertices());

	m_d3d_vertexBuffer = m_d3d->CreateVertexBuffer();

    HRESULT hres = m_d3d_device->GetSwapChain(0, &m_d3d_swapChain);   
	Assert(hres == D3D_OK);
    
	m_dwAlphaBlendable = 0;
    m_dwSrcBlend       = 0;
    m_dwDstBlend       = 0;
    m_bStripMode       = true;
    m_id3dx_font       = nullptr;
    SetFontSize(9_PIXEL);
}

void D3D_driver::ShutDown()
{

}

PixelRect D3D_driver::CalcGraphicsRect(std::wstring const & wstr)
{
	RECT rect{ 0, 0, 0, 0 };
    Assert(m_id3dx_font != nullptr);
    m_id3dx_font->DrawText
    (
        nullptr,           // pSprite
        wstr.c_str(),     // pString
        -1,                // Count
        &rect,             // pRect
        DT_CALCRECT,       // Format
        0                  // Color
   );
	return RECT2PixelRect(rect);
}

void D3D_driver::DisplayGraphicsText
(
	PixelRect const & pixRect, 
	wstring   const & wstr,
	DWORD     const   format,
	COLORREF  const   col 
)
{
	RECT rect(PixelRect2RECT(pixRect));			  
    Assert(m_id3dx_font != nullptr);
    m_id3dx_font->DrawText
    (
        nullptr,           // pSprite
        wstr.c_str(),     // pString
        -1,                // Count
        &rect,             // pRect
		format,            // Format
		COLORREFtoD3DCOLOR(255, col)  
   ); 
}

// functions called per frame

bool D3D_driver::StartFrame(HWND const hwnd, HDC const hdc)
{
    HRESULT hres;
	m_d3d->ResizeD3dSystem(hwnd); 
    hres = m_d3d_device->SetRenderState(D3DRS_LIGHTING, false);                  if (hres != D3D_OK) return false;
    hres = m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);           if (hres != D3D_OK) return false;
    hres = m_d3d_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);                    if (hres != D3D_OK) return false;
    hres = m_d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET, CLR_WHITE, 1.0f, 0); if (hres != D3D_OK) return false;
    hres = m_d3d_device->BeginScene();                                            if (hres != D3D_OK) return false;
    if (m_bStripMode)
        m_pVertBufStripMode->ResetVertexBuffer();
    m_pVertBufPrimitives->ResetVertexBuffer();
    if (! setFont())
		return false;
	return true;
}

bool D3D_driver::setFont()
{
    HRESULT const hres = D3DXCreateFontIndirect(m_d3d_device, &m_d3dx_font_desc, &m_id3dx_font);   
    if (hres != D3D_OK)          return false;
    if (m_id3dx_font == nullptr) return false;
	return true;
}

void D3D_driver::SetFontSize(PIXEL const nPointSize)
{
    //
    // To create a Windows friendly font using only a point size, an 
    // application must calculate the logical height of the font.
    // 
    // This is because functions like CreateFont() and CreateFontIndirect() 
    // only use logical units to specify height.
    //
    // Here's the formula to find the height in logical pixels:
    //
    //             -(point_size * LOGPIXELSY)
    //    height = ----------------------------
    //                          72
    //

    HDC const hDC = GetDC(nullptr);
    int const iLogPixels = GetDeviceCaps(hDC, LOGPIXELSY);
    ReleaseDC(nullptr, hDC);
    m_d3dx_font_desc.Height = -(MulDiv(nPointSize.GetValue(), iLogPixels, 72));
	m_d3dx_font_desc.Width  = m_d3dx_font_desc.Height / 2;
    setFont();   
}

void D3D_driver::addRect2Buffer(float const fWest, float const fNorth, float const fEast, float const fSouth, D3DCOLOR const dwColor)
{    
    m_pVertBufPrimitives->AddVertex(fWest, fNorth, dwColor);
    m_pVertBufPrimitives->AddVertex(fEast, fNorth, dwColor);
    m_pVertBufPrimitives->AddVertex(fEast, fSouth, dwColor);
    m_pVertBufPrimitives->AddVertex(fWest, fSouth, dwColor);
}

void D3D_driver::addRectangle(float const fPtPosx, float const fPtPosy, D3DCOLOR const dwColor, float const fPixSize)
{
    addRect2Buffer
    (
        fPtPosx - fPixSize, 
        fPtPosy - fPixSize, 
        fPtPosx + fPixSize, 
        fPtPosy + fPixSize, 
        dwColor 
   );
}

void D3D_driver::addHexagon(float const fPtPosx, float const fPtPosy, D3DCOLOR const dwColor, float const fPixSizeX, float const fPixSizeY)
{
	m_pVertBufPrimitives->AddVertex(fPtPosx - fPixSizeX     - 0.5f, fPtPosy,                 dwColor);     // west
	m_pVertBufPrimitives->AddVertex(fPtPosx - fPixSizeX / 2 - 0.5f, fPtPosy - fPixSizeY / 2, dwColor);     // north west
	m_pVertBufPrimitives->AddVertex(fPtPosx - fPixSizeX / 2 - 0.5f, fPtPosy + fPixSizeY / 2, dwColor);     // south west
	m_pVertBufPrimitives->AddVertex(fPtPosx + fPixSizeX / 2 + 0.5f, fPtPosy - fPixSizeY / 2, dwColor);     // north east
 	m_pVertBufPrimitives->AddVertex(fPtPosx + fPixSizeX / 2 + 0.5f, fPtPosy + fPixSizeY / 2, dwColor);     // south east
	m_pVertBufPrimitives->AddVertex(fPtPosx + fPixSizeX     + 0.5f, fPtPosy,                 dwColor);     // east
}

void D3D_driver::AddIndividual(PixelPoint const ptPos, COLORREF const color, float const fPixSize)
{
	static float const SQRT3 = static_cast<float>(sqrt(3));

    float const fPtPosx = static_cast<float>(ptPos.GetXvalue());
    float const fPtPosy = static_cast<float>(ptPos.GetYvalue());

	D3DCOLOR const D3Dcolor = color; // COLORREFtoD3DCOLOR(255, color);

    if (m_bStripMode)
    {
		addRectangle(fPtPosx, fPtPosy, D3Dcolor, fPixSize);
	}
	else
	{
		if (m_d3d->IsHexagonMode())
			addHexagon(fPtPosx, fPtPosy, D3Dcolor, fPixSize, SQRT3 * fPixSize);
		else
			addRectangle(fPtPosx, fPtPosy, D3Dcolor, fPixSize);
	}
}

void D3D_driver::AddBackGround(PixelPoint const ptPos, COLORREF const color, float const fPixSize)
{
	static float const INVERSE_SQRT3 = static_cast<float>(1 / sqrt(3));

	float const fPtPosx = static_cast<float>(ptPos.GetXvalue());
    float const fPtPosy = static_cast<float>(ptPos.GetYvalue());

	float const fPixSizeHalf = fPixSize / 2;

	D3DCOLOR const D3Dcolor = color; // COLORREFtoD3DCOLOR(255, color);

    if (m_bStripMode)
    {
        m_pVertBufStripMode->AddVertex(fPtPosx, fPtPosy, D3Dcolor);
    }
    else
    {
		if (m_d3d->IsHexagonMode())
			addHexagon(fPtPosx, fPtPosy, D3Dcolor, fPixSize * INVERSE_SQRT3, fPixSize);
		else
			addRectangle(fPtPosx, fPtPosy, D3Dcolor, fPixSizeHalf);
    }
}

void D3D_driver::prepareTranspMode()
{
    m_d3d_device->GetRenderState(D3DRS_ALPHABLENDENABLE, & m_dwAlphaBlendable);
    m_d3d_device->GetRenderState(D3DRS_SRCBLEND,         & m_dwSrcBlend);
    m_d3d_device->GetRenderState(D3DRS_DESTBLEND,        & m_dwDstBlend);    

    m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    m_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    

    m_pVertBufPrimitives->ResetVertexBuffer();
}

void D3D_driver::finishTranspMode()
{
    m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, m_dwAlphaBlendable);
    m_d3d_device->SetRenderState(D3DRS_SRCBLEND, m_dwSrcBlend);
    m_d3d_device->SetRenderState(D3DRS_DESTBLEND, m_dwDstBlend);    
}

void D3D_driver::RenderTranspRect
(
	PixelRect    const & rectTransparent, 
	unsigned int const   uiALpha,
	COLORREF     const   color
)
{
	if (rectTransparent.IsNotEmpty())
	{
		Assert(m_d3d_device != nullptr);
		prepareTranspMode();

		addRect2Buffer
		(
			static_cast<float>(rectTransparent.GetLeft  ().GetValue()), 
			static_cast<float>(rectTransparent.GetTop   ().GetValue()), 
			static_cast<float>(rectTransparent.GetRight ().GetValue()), 
			static_cast<float>(rectTransparent.GetBottom().GetValue()), 
			COLORREFtoD3DCOLOR(uiALpha, color) 
		);

		renderPrimitives(m_d3d->GetRectIndexBuffer());

		finishTranspMode();
	}
}

void D3D_driver::RenderBackground()
{
    if (m_bStripMode)
        renderIndexedTriangleStrip();
    else
        renderPrimitives(m_d3d->GetIndsIndexBuffer());
}

void D3D_driver::RenderForegroundObjects()
{
    renderPrimitives(m_d3d->GetIndsIndexBuffer());
}

void D3D_driver::renderIndexedTriangleStrip() const
{
	HRESULT hres;

    Assert(m_pVertBufStripMode->GetNrOfVertices() > 0);
    Assert(m_d3d_device != nullptr);

    D3dIndexBuffer const * const iBuf = m_d3d->GetBgIndexBufferStripMode();
    
    hres = m_pVertBufStripMode->LoadVertices(m_d3d_vertexBuffer, m_d3d_device);

	iBuf->SetIndices(m_d3d_device);
    
	ULONG ulNrOfVertices   = m_pVertBufStripMode->GetNrOfVertices();
	UINT  uiNrOfPrimitives = ulNrOfVertices - 2;
	
	hres = m_d3d_device->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP, 
		0, 
		0, 
		m_pVertBufStripMode->GetNrOfVertices(), 
		0, 
		iBuf->GetMaxNrOfPrimitives() 
	);

    Assert(hres == D3D_OK);
}

void D3D_driver::renderPrimitives(D3dIndexBuffer const * const iBuf)
{
	ULONG const ulNrOfPrimitives = m_d3d->GetTrianglesPerPrimitive() * m_pVertBufPrimitives->GetNrOfVertices() / 
		                           m_d3d->GetVerticesPerPrimitive();
	
	if (ulNrOfPrimitives > 0)
    {
		HRESULT hres = m_pVertBufPrimitives->LoadVertices(m_d3d_vertexBuffer, m_d3d_device);
        Assert(hres == D3D_OK); 

        iBuf->SetIndices(m_d3d_device);
        hres = m_d3d_device->DrawIndexedPrimitive
		(
			D3DPT_TRIANGLELIST, 
			0, 
			0, 
			m_pVertBufPrimitives->GetNrOfVertices(), 
			0, 
			ulNrOfPrimitives 
		); 
        Assert(hres == D3D_OK); 
        m_pVertBufPrimitives->ResetVertexBuffer();
    }
}

// Finish rendering; page flip.

void D3D_driver::EndFrame(HWND const hwnd)
{
    HRESULT hres;
    hres = m_d3d_device->EndScene();                                       Assert(hres == D3D_OK);
	hres = m_d3d_swapChain->Present(nullptr, nullptr, hwnd, nullptr, 0); Assert(hres == D3D_OK);
    m_id3dx_font->Release();      
}

D3DCOLOR D3D_driver::COLORREFtoD3DCOLOR(unsigned int const uiALpha, COLORREF const color)
{
	UINT const uiR = GetRValue (color); 
	UINT const uiG = GetGValue (color); 
	UINT const uiB = GetBValue (color);

	return D3DCOLOR_ARGB(uiALpha, uiR, uiG, uiB);
}

///// NNet support ---- experimental ----- ////////////////////

void D3D_driver::AddfPixelLine
(
	fPixelPoint const & fpp1, 
	fPixelPoint const & fpp2, 
	fPixel      const   fpixWidth, 
	COLORREF    const   color
)
{
	D3DCOLOR    const D3Dcolor      { COLORREFtoD3DCOLOR(255, color) };
	fPixelPoint const fPixPntVec    { fpp1 - fpp2 };
	fPixelPoint const fPixPntScaled { fPixPntVec.ScaledTo(fpixWidth) };
	fPixelPoint const fOrthoScaled  { fPixPntScaled.OrthoVector() };
	m_pVertBufPrimitives->AddVertex(Cast2Float(fpp1.GetXvalue() + fOrthoScaled.GetXvalue()), Cast2Float(fpp1.GetYvalue() + fOrthoScaled.GetYvalue()), D3Dcolor);
	m_pVertBufPrimitives->AddVertex(Cast2Float(fpp1.GetXvalue() - fOrthoScaled.GetXvalue()), Cast2Float(fpp1.GetYvalue() - fOrthoScaled.GetYvalue()), D3Dcolor);
	m_pVertBufPrimitives->AddVertex(Cast2Float(fpp2.GetXvalue() - fOrthoScaled.GetXvalue()), Cast2Float(fpp2.GetYvalue() - fOrthoScaled.GetYvalue()), D3Dcolor);
	m_pVertBufPrimitives->AddVertex(Cast2Float(fpp2.GetXvalue() + fOrthoScaled.GetXvalue()), Cast2Float(fpp2.GetYvalue() + fOrthoScaled.GetYvalue()), D3Dcolor);
}

void D3D_driver::renderTriangleStrip(int const iNrOfPrimitives)
{
	HRESULT hres = m_pVertBufStripMode->LoadVertices(m_d3d_vertexBuffer, m_d3d_device); Assert(hres == D3D_OK); 
	hres = m_d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, iNrOfPrimitives);        Assert(hres == D3D_OK); 
	m_pVertBufStripMode->ResetVertexBuffer();
}

void D3D_driver::StartPipeline
(
	fPixelPoint const & fppStart, 
	fPixelPoint const & fppEnd, 
	fPixel      const   fpixWidth, 
	COLORREF    const   color
)
{
	fPixelPoint const fPixPntVec    { fppEnd - fppStart };
	fPixelPoint const fPixPntScaled { fPixPntVec.ScaledTo(fpixWidth) };
	m_fOrtho = fPixPntScaled.OrthoVector();
	m_pVertBufStripMode->ResetVertexBuffer();
	AddPipelinePoint(fppStart, color);
}

void D3D_driver::AddPipelinePoint
(
	fPixelPoint const & fpp, 
	COLORREF    const   color
)
{
	D3DCOLOR D3Dcolor = COLORREFtoD3DCOLOR(255, color);

	m_pVertBufStripMode->AddVertex(Cast2Float(fpp.GetXvalue() + m_fOrtho.GetXvalue()), Cast2Float(fpp.GetYvalue() + m_fOrtho.GetYvalue()), D3Dcolor);
	m_pVertBufStripMode->AddVertex(Cast2Float(fpp.GetXvalue() - m_fOrtho.GetXvalue()), Cast2Float(fpp.GetYvalue() - m_fOrtho.GetYvalue()), D3Dcolor);
}

void D3D_driver::RenderPipeline()
{
	renderTriangleStrip(m_pVertBufStripMode->GetNrOfVertices() - 2);
}

void D3D_driver::DrawPolygon
(
	int         const iNrOfEdges,
	fPixelPoint const ptPos,
	COLORREF    const color, 
	fPixel      const fPixRadius 
)
{
    float STEP = 2.0f * D3DX_PI / iNrOfEdges;

	D3DCOLOR const D3Dcolor = COLORREFtoD3DCOLOR(255, color);

	m_pVertBufStripMode->AddVertex
	(
		ptPos.GetXvalue() - fPixRadius.GetValue(),
		ptPos.GetYvalue(),
		D3Dcolor 
	);

	for (float f = 0.0; f <= STEP * iNrOfEdges / 2; f += STEP)
	{
		float fX = fPixRadius.GetValue() * cosf(f);
		float fY = fPixRadius.GetValue() * sinf(f);
		m_pVertBufStripMode->AddVertex
		(
			ptPos.GetXvalue() + fX, 
			ptPos.GetYvalue() + fY,	
			D3Dcolor 
		);

		m_pVertBufStripMode->AddVertex
		(
			ptPos.GetXvalue() + fX, 
			ptPos.GetYvalue() - fY,	
			D3Dcolor 
		);
	}

	m_pVertBufStripMode->AddVertex
	(
		ptPos.GetXvalue() + fPixRadius.GetValue(),
		ptPos.GetYvalue(),
		D3Dcolor 
	);

	renderTriangleStrip(iNrOfEdges);
}
