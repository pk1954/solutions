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

	D2D_driver* GetGraphics() { return m_upGraphics.get(); }

	virtual void Stop();

	D2D1_COLOR_F SetForegroundColor(D2D1_COLOR_F const c) { return m_upGraphics->SetForegroundColor(c); }
	D2D1_COLOR_F SetBackgroundColor(D2D1_COLOR_F const c) { return m_upGraphics->SetBackgroundColor(c); }

	D2D1_COLOR_F SetForegroundColor(COLORREF const c) { return m_upGraphics->SetForegroundColor(c); }
	D2D1_COLOR_F SetBackgroundColor(COLORREF const c) { return m_upGraphics->SetBackgroundColor(c); }

	D2D1_COLOR_F GetForegroundColor() const { return m_upGraphics->GetForegroundColor(); }
	D2D1_COLOR_F GetBackgroundColor() const { return m_upGraphics->GetBackgroundColor(); }

	COLORREF SetBackgroundColorRef(COLORREF const c) final { return Convert2COLORREF(SetBackgroundColor(c)); }
	COLORREF GetBackgroundColorRef() const           final { return Convert2COLORREF(GetBackgroundColor()); }

protected:

	HWND Initialize(HWND const, LPCTSTR const, DWORD const);

	void OnPaint() override;
	bool OnSize(PIXEL const, PIXEL const) override;

	virtual void PaintGraphics() = 0;

	unique_ptr<D2D_driver> m_upGraphics { nullptr };
};