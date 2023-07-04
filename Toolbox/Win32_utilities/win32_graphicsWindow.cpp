// Win32_GraphicsWindow.cpp : 
//
// Win32_utilities

module;

#include <memory>
#include <functional>
#include <Windows.h>

module GraphicsWindow;

import Direct2D;
import Types;
import BaseWindow;

using std::bind_front;
using std::make_unique;

HWND GraphicsWindow::Initialize
(
	HWND    const hwndParent,
	LPCTSTR const szClass,
	DWORD   const dwWindowStyle
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		szClass,
		dwWindowStyle,
		nullptr,
		nullptr
	);
	m_upGraphics = D2D_driver::Create(hwnd);
	return hwnd;
}

void GraphicsWindow::Stop()
{
	BaseWindow::Stop();
	m_upGraphics->ShutDown();
}

void GraphicsWindow::OnPaint()
{
	if (IsWindowVisible())
		m_upGraphics->Display(bind_front(&GraphicsWindow::PaintGraphics, this));
}

bool GraphicsWindow::OnSize(PIXEL const width, PIXEL const height)
{
	if (m_upGraphics)
		m_upGraphics->Resize();
	return true;
}
