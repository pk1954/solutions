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

	Color SetForegroundColor(Color const c) { return m_upGraphics->SetForegroundColor(c); }
	Color SetBackgroundColor(Color const c) { return m_upGraphics->SetBackgroundColor(c); }

	Color GetForegroundColor() const { return m_upGraphics->GetForegroundColor(); }
	Color GetBackgroundColor() const { return m_upGraphics->GetBackgroundColor(); }

	LPARAM AddContextMenuEntries(HMENU const) override;

protected:

	HWND Initialize(HWND const, LPCTSTR const, DWORD const);

	void OnPaint() override;
	bool OnSize(PIXEL const, PIXEL const) override;

	virtual void PaintGraphics() = 0;

	unique_ptr<D2D_driver> m_upGraphics { nullptr };
};