// Win32_GraphicsWindow.cpp : 
//
// Win32_utilities

module GraphicsWindow;

import std;
import WinBasics;
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
		szClass,
		dwWindowStyle,
		nullptr,
		nullptr
	);
	m_upGraphics = D2D_driver::Create(hwnd);
	return hwnd;
}

void GraphicsWindow::OnPaint()
{
	if (IsWindowVisible())
		m_upGraphics->Display(bind_front(&GraphicsWindow::PaintGraphics, this));
}

bool GraphicsWindow::OnSize(PIXEL const width, PIXEL const height)
{
	if (m_upGraphics)
		m_upGraphics->Resize(width, height);
	return true;
}

LPARAM GraphicsWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AddColorCtlMenu(hPopupMenu);
	return 0;
}

void GraphicsWindow::SetForegroundColor(Color const c) 
{ 
	m_upGraphics->SetForegroundColor(c);
	Trigger();
}

void GraphicsWindow::SetBackgroundColor(Color const c) 
{ 
	m_upGraphics->SetBackgroundColor(c);
	Trigger();
}
