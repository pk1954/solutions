// win32_graphicsWindow.h : 
//
// Win32_utilities

#pragma once

#include "win32_baseWindow.h"

using std::unique_ptr;

class D2D_driver;

class GraphicsWindow : public BaseWindow
{
public:
	virtual void Stop();

protected:

	virtual void Reset();

	void Initialize(HWND const, LPCTSTR const, DWORD const);
	void TrackMouse();

	void OnPaint() override;
	bool OnSize(WPARAM const, LPARAM const) override;

	virtual void DoPaint() = 0;

	unique_ptr<D2D_driver> m_upGraphics { };

private:
	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };
};