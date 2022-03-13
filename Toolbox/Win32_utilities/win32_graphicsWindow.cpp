// win32_graphicsWindow.cpp : 
//
// Win32_utilities

#include "stdafx.h"
#include "Direct2D.h"
#include "win32_graphicsWindow.h"

using std::make_unique;

HWND GraphicsWindow::Initialize
(
	HWND    const hwndParent,
	LPCTSTR const szClass,
	DWORD   const dwWindowStyle
)
{
	m_upGraphics = make_unique<D2D_driver>();
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		szClass,
		dwWindowStyle,
		nullptr,
		nullptr
	);
	m_upGraphics->Initialize(hwnd);
	m_trackStruct.hwndTrack = hwnd;
	return hwnd;
}

void GraphicsWindow::Reset()
{
	m_trackStruct.hwndTrack = HWND(0);
	(void)TrackMouseEvent(& m_trackStruct);
}

void GraphicsWindow::Stop()
{
	Reset();
	m_upGraphics->ShutDown();
	DestroyWindow();
}

void GraphicsWindow::TrackMouse()
{
	(void)TrackMouseEvent(& m_trackStruct);
}

void GraphicsWindow::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		if (m_upGraphics->StartFrame())
		{
			DoPaint();
			m_upGraphics->EndFrame();
		}
		EndPaint(&ps);
	}
}

bool GraphicsWindow::OnSize(PIXEL const width, PIXEL const height)
{
	m_upGraphics->Resize(width, height);
	return true;
}
