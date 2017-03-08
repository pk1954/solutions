// DirectX-test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DirectX-test.h"

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <assert.h>

HWND hwnd;
WNDCLASSEX the_wndclass;
HINSTANCE the_hinstance;

IDirect3D9 *d3d_object;
IDirect3DDevice9 *d3d_device;

struct Line_Vertex 
{
    float x, y, z;
    unsigned long color;
};

class GRIDPOINT
{
public:
	long x;
	long y;
	inline bool operator== (GRIDPOINT a) { return (a.x == x) && (a.y == y); };
	inline bool operator!= (GRIDPOINT a) { return (a.x != x) || (a.y != y); };
	inline GRIDPOINT operator+ (GRIDPOINT a) { GRIDPOINT gpSum; gpSum.x = x + a.x; gpSum.y = y + a.y; return gpSum; };
	inline GRIDPOINT operator% (GRIDPOINT a) { GRIDPOINT gpMod; gpMod.x = x % a.x; gpMod.y = y % a.y; return gpMod; };

	static const int GRID_WIDTH  = 600;
	static const int GRID_HEIGHT = 300;
	static const int GRID_AREA   = GRID_WIDTH * GRID_HEIGHT;

	static const GRIDPOINT GP_NULL; 
	static const GRIDPOINT GRID_ORIGIN;
	static const GRIDPOINT GRID_SIZE;
	static const RECT	   GRID_RECT;
};

const RECT GRIDPOINT::GRID_RECT = { 0, 0, GRIDPOINT::GRID_WIDTH, GRIDPOINT::GRID_HEIGHT };

class GridField
{
public:
    int m_iFoodStock;
    int m_iFoodMax;
};

static const long m_lFieldSize = 1;

GridField m_aGF[GRIDPOINT::GRID_WIDTH][GRIDPOINT::GRID_HEIGHT];

//
// rendering_2d() sets the Direct3D transformation matrix for 2D rendering.
// In an API geared toward 2D rendering you could probably just delete this function.
//
void rendering_2d( HWND hwnd ) 
{
    D3DMATRIX m;
    RECT rect;
    GetClientRect(hwnd, &rect);

    int width  = rect.right  - rect.left;
    int height = rect.bottom - rect.top;

    memset(&m, 0, sizeof(D3DMATRIX));

    m._11 =  2.0f / (float)width;
    m._41 = -1.0f;
    m._22 =  2.0f / (float)height;
    m._42 = -1.0f;

    m._33 = 1;
    m._43 = 0;

    m._34 = 0;
    m._44 = 1;

    d3d_device->SetTransform(D3DTS_PROJECTION, &m);
}

static const long m_xOffset = 0;
static const long m_yOffset = 0;

POINT Grid2PixelPos( GRIDPOINT gp )
{
	POINT ptPixel = 
	{ 
		gp.x * m_lFieldSize - m_xOffset,
		gp.y * m_lFieldSize - m_yOffset
	};
	
	return ptPixel;
}

GRIDPOINT Pixel2GridPos( POINT ptPixel )
{
	GRIDPOINT gp = 
	{ 
		(ptPixel.x + m_xOffset) / m_lFieldSize,
		(ptPixel.y + m_yOffset) / m_lFieldSize
	};
	return gp;
}

RECT Pixel2GridRect( RECT rcClient )
{
	POINT ptPixelStart = { rcClient.left,  rcClient.top    };
	POINT ptPixelEnd   = { rcClient.right, rcClient.bottom };

	GRIDPOINT pgStart  = Pixel2GridPos( ptPixelStart );
	GRIDPOINT pgEnd    = Pixel2GridPos( ptPixelEnd   );

	RECT rcGrid = { pgStart.x, pgStart.y, pgEnd.x, pgEnd.y };

	return rcGrid;
}

void drawRect( POINT ptNE, int iLevel, long lPxSize )
{
    Line_Vertex p[4];
    p[0].x = p[3].x = (float)(ptNE.x);
    p[1].x = p[2].x = (float)(ptNE.x + lPxSize);

    p[0].y = p[1].y = (float)(ptNE.y);
    p[2].y = p[3].y = (float)(ptNE.y + lPxSize);

    p[0].z = p[1].z = p[2].z = p[3].z = 0;
    p[0].color = p[1].color = p[2].color = p[3].color = RGB( iLevel, iLevel, iLevel );            
    HRESULT result = d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, p, sizeof(Line_Vertex) );  // Half the rectangle
}

void drawFood( RECT *prect )
{
    static const long MIN_FIELD_SIZE = 1;
    long  lFactor  = (m_lFieldSize < MIN_FIELD_SIZE) ? MIN_FIELD_SIZE / m_lFieldSize : 1;
    int   iMaxFood = 2000;
    long  lPxSize  = m_lFieldSize * lFactor;

    if (!d3d_device) return;  // Haven't been initialized yet!

    HRESULT result = d3d_device->BeginScene();
    if (result != D3D_OK) return;

    unsigned long clear_color = 0xffffffff;
    d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, clear_color, 1.0f, 0);
    d3d_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);

    GRIDPOINT gp;
	for (gp.x = prect->left; gp.x < prect->right; gp.x += lFactor)
    {
		for (gp.y = prect->top; gp.y < prect->bottom; gp.y += lFactor)
		{
            GRIDPOINT gpRun;
            long lSum = 0;
	        for (gpRun.x = gp.x; gpRun.x < gp.x + lFactor; gpRun.x++)
		        for (gpRun.y = gp.y; gpRun.y < gp.y + lFactor; gpRun.y++)
                    lSum += m_aGF[gpRun.x][gpRun.y].m_iFoodStock;
            int   iFood      =  lSum / (lFactor * lFactor);
	        int   iFoodLevel = (iFood * 255) / iMaxFood;
            POINT ptNE       = Grid2PixelPos( gp );
            drawRect( ptNE, iFoodLevel, lPxSize ); 
        }
    }

    // Finish rendering; page flip.
    HRESULT hr = d3d_device->EndScene();
    hr = d3d_device->Present(NULL, NULL, NULL, NULL);
}

bool draw() 
{
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);
	RECT rcGrid = Pixel2GridRect( rcClient );

    InflateRect( &rcGrid, 1, 1 );
	IntersectRect( &rcGrid, &rcGrid, &GRIDPOINT::GRID_RECT );

    drawFood( &rcGrid );

    return true;
}

// Fill the struct that tells D3D how to set up buffers and page flip.
void fill_out_present(D3DPRESENT_PARAMETERS *present, HWND hwnd) 
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width  = rect.right - rect.left;
    int height = rect.bottom - rect.top;

	ZeroMemory(present, sizeof(*present));

    present->Windowed               = TRUE;
    present->MultiSampleType        = D3DMULTISAMPLE_NONE;
    present->SwapEffect             = D3DSWAPEFFECT_DISCARD;
    present->EnableAutoDepthStencil = FALSE;
    present->BackBufferFormat       = D3DFMT_X8R8G8B8;
    present->hDeviceWindow          = hwnd;
    present->Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    present->BackBufferCount        = 1;
    present->BackBufferWidth        = width;
    present->BackBufferHeight       = height;
    present->PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
}


/******************************************************************
*                                                                 *
*  WndProc                                                        *
*                                                                 *
*  Window message handler                                         *
*                                                                 *
******************************************************************/
LRESULT CALLBACK MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_PAINT:
        case WM_DISPLAYCHANGE:
            {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);
                draw();
                EndPaint(hwnd, &ps);
            }
            return 0;
        case WM_SIZE:
            {
                if (d3d_device) 
                {
                    D3DPRESENT_PARAMETERS present;
                    fill_out_present(&present, hwnd);
                    if (present.BackBufferWidth && present.BackBufferHeight) 
                    {
                        d3d_device->Reset(&present);
                        InvalidateRect(hwnd, NULL, FALSE);
                    }
                    rendering_2d( hwnd );
                }
            }
            break;
        case WM_DESTROY:
            {
                d3d_device->Release();    // close and release the 3D device
                d3d_object->Release();    // close and release Direct3D
                PostQuitMessage(0);
            }
            return 1;
    }

    return DefWindowProc( hwnd, message, wParam, lParam );
}

void SetFoodMaxSquare( long left, long top, long right, long bottom, long lVal )
{
	GRIDPOINT gp;
	for (gp.x = left; gp.x < right; gp.x++)
		for (gp.y = top; gp.y < bottom; gp.y++)
		{
            GridField *pGF = &m_aGF[gp.x][gp.y];
            pGF->m_iFoodStock = lVal;
			pGF->m_iFoodMax   = lVal;
		}
}

void SetFoodMaxCone( long lCenterX, long lCenterY, long lRadius, long lValCenter, long lValEdge )
{
	GRIDPOINT gp;
    long lLeft      = lCenterX - lRadius;
    long lTop       = lCenterY - lRadius;
    long lRight     = lCenterX + lRadius;
    long lBottom    = lCenterY + lRadius; 
    long lRadSquare = lRadius * lRadius;
    long lValDelta  = lValCenter - lValEdge;
	for (gp.x = lLeft; gp.x < lRight; gp.x++)
		for (gp.y = lTop; gp.y < lBottom; gp.y++)
		{
            long lDistX = gp.x - lCenterX;
            long lDistY = gp.y - lCenterY;
            long lDistSquare = lDistX * lDistX + lDistY * lDistY;
            if ( lDistSquare <= lRadSquare )
            { 
                long lVal = lValCenter - (lDistSquare * lValDelta) / lRadSquare;
                GridField *pGF = &m_aGF[gp.x][gp.y];
                if ( lVal > pGF->m_iFoodStock )
                {
                    pGF->m_iFoodStock = lVal;
			        pGF->m_iFoodMax   = lVal;
                }
            }
		}
}

void InitializeFood( )
{
	long lMax    = 2000;
	long lMin    = 50;
	long lWidth  = GRIDPOINT::GRID_WIDTH;
	long lDist   = GRIDPOINT::GRID_WIDTH / 20;
    long lMidX   = GRIDPOINT::GRID_WIDTH / 2;
    long lHeight = GRIDPOINT::GRID_HEIGHT;
    long lMidY   = GRIDPOINT::GRID_HEIGHT / 2;

    assert( lHeight > lDist );

    SetFoodMaxSquare( 0, 0, lWidth, lHeight, lMin );
    
    long lRadius = (lHeight - lDist) / 2;
   
	SetFoodMaxCone(          lDist + lRadius, lHeight / 2, lRadius, lMax, lMin );  // left 
	SetFoodMaxCone( lWidth - lDist - lRadius, lHeight / 2, lRadius, lMax, lMin );  // right

	lMax = 0;
}

void register_window_class(LPCWSTR winclass_name) {
    WNDCLASSEX wcl = { sizeof(WNDCLASSEX), CS_CLASSDC, MessageProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, (LPCWSTR)winclass_name, NULL };
    the_wndclass = wcl;

    the_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    int success = RegisterClassEx(&the_wndclass);
	assert(success);
}

bool initialize() {

    LPCWSTR name = _T("Not Using Direct2D");

    register_window_class( name );

    hwnd = CreateWindow
    (
        name,
        name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        the_hinstance,
        NULL
    );
    
    d3d_object = Direct3DCreate9(D3D_SDK_VERSION);

    // Fill out the presentation parameters for the D3D device... windowed mode.
    D3DPRESENT_PARAMETERS present;
    fill_out_present(&present, hwnd);

    UINT AdapterToUse = D3DADAPTER_DEFAULT;
    D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
    d3d_object->CreateDevice
    (
        AdapterToUse, 
        DeviceType, 
        hwnd,
        D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &present, 
        &d3d_device
    );

    if (!d3d_device) return false;

    D3DCAPS9 Caps;
    HRESULT hres = d3d_object->GetDeviceCaps( AdapterToUse, DeviceType, &Caps );

    InitializeFood( );

    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hwnd);

    return true;
}

/******************************************************************
*                                                                 *
*  WinMain                                                        *
*                                                                 *
*  Application entrypoint                                         *
*                                                                 *
******************************************************************/
int WINAPI WinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    the_hinstance = hInstance;

    initialize();

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

