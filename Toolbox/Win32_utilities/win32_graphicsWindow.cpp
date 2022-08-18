// Win32_GraphicsWindow.cpp : 
//
// Win32_utilities

module;

#include <memory>
#include <Windows.h>

module GraphicsWindow;

import Direct2D;
import PixelTypes;
import BaseWindow;

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
	m_upGraphics->InitWindow(hwnd);
	return hwnd;
}

void GraphicsWindow::Stop()
{
	m_upGraphics->ShutDown();
	DestroyWindow();
}

void GraphicsWindow::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		if (m_upGraphics->StartFrame())
		{
			m_upGraphics->FillBackground(D2D1::ColorF::Azure);
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
