// win32_graphicsWindow.h : 
//
// Win32_utilities
//
// thin layer on top of BaseWindow
// providing D2D graphics capability

#pragma once

#include "win32_baseWindow.h"

using std::unique_ptr;

class D2D_driver;

class GraphicsWindow : public BaseWindow
{
public:
	GraphicsWindow() = default;

	virtual void Stop();

protected:

	HWND Initialize(HWND const, LPCTSTR const, DWORD const);

	void OnPaint() override;
	bool OnSize(PIXEL const, PIXEL const) override;

	virtual void DoPaint() = 0;

	unique_ptr<D2D_driver> m_upGraphics { nullptr };
};