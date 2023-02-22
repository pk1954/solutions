// Win32_GraphicsWindow.ixx
//
// Win32_utilities
//
// thin layer on top of BaseWindow
// providing D2D graphics capability

module;

#include <memory>
#include <Windows.h>

export module GraphicsWindow;

import Direct2D;
import Types;
import BaseWindow;

using std::unique_ptr;

export class GraphicsWindow : public BaseWindow
{
public:
	GraphicsWindow() = default;
	~GraphicsWindow() override = default;

	virtual void Stop();

protected:

	HWND Initialize(HWND const, LPCTSTR const, DWORD const);

	void OnPaint() override;
	bool OnSize(PIXEL const, PIXEL const) override;

	virtual void DoPaint() = 0;

	fPixel GetClientHeight() const { return Convert2fPixel(GetClientWindowHeight()); }
	fPixel GetClientWidth () const { return Convert2fPixel(GetClientWindowWidth()); }

	unique_ptr<D2D_driver> m_upGraphics{ nullptr };
};