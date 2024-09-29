// Win32_BaseWindow.ixx
//
// Win32_utilities

export module BaseWindow;

import std;
import Types;
import BoolType;
import HiResTimer;
import WinBasics;
import RootWindow;

using std::wstring;

static LRESULT __stdcall BaseWndProc(HWND const, UINT const, WPARAM const, LPARAM const);

export class BaseWindow : public RootWindow
{
public:

	~BaseWindow() override = default;

	HWND StartBaseWindow
	(
		HWND      const, 
		LPCTSTR   const, 
		DWORD     const, 
		PixelRect const *,
		VisCrit   const &
	);

	void SetCaption() const;

	virtual wstring GetCaption() const;

	inline static BoolType m_bPerfMonMode { false };

protected:

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

	virtual void OnDrawItem         (WPARAM const, DRAWITEMSTRUCT const * const) { /* empty */ }
	virtual void OnScaleCommand     (WPARAM const, LPARAM const) { /* empty */ }
	virtual void OnPaint            ()                           { /* empty */ }
	virtual void OnLButtonDblClick  (WPARAM const, LPARAM const) { /* empty */ }
	virtual void OnNCLButtonDblClick(WPARAM const, LPARAM const) { /* empty */ }
	virtual void OnChar             (WPARAM const, LPARAM const) { /* empty */ }
	virtual void OnNotify           (WPARAM const, LPARAM const) { /* empty */ }
	virtual bool OnMouseWheel       (WPARAM const, LPARAM const) { return false; }
	virtual bool OnRButtonDown      (WPARAM const, LPARAM const) { return false; }
	virtual bool OnRButtonUp        (WPARAM const, LPARAM const) { return false; }
	virtual bool OnLButtonDown      (WPARAM const, LPARAM const) { return false; }
	virtual bool OnLButtonUp        (WPARAM const, LPARAM const) { return false; }
	virtual bool OnShow             (WPARAM const, LPARAM const) { return false; }
	virtual bool OnSetCursor        (WPARAM const, LPARAM const) { return false; }
	virtual bool OnTimer            (WPARAM const, LPARAM const) { return false; }
	virtual bool OnCtlColorStatic   (WPARAM const, LPARAM const) { return false; }
	virtual void OnMouseHover       (WPARAM const, LPARAM const);
	virtual void OnMouseMove        (WPARAM const, LPARAM const);
	virtual void OnMouseLeave       ();

	fPixel GetClientHeight() const { return Convert2fPixel(GetClientWindowHeight()); }
	fPixel GetClientWidth () const { return Convert2fPixel(GetClientWindowWidth()); }

	bool OnSize(PIXEL const, PIXEL const) override { return false; };

private:

	void trackMouse(bool const) const;

	friend static LRESULT __stdcall BaseWndProc(HWND const, UINT const, WPARAM const, LPARAM const);

	HiResTimer m_paintTimer;
	fMicroSecs m_usPaintTime { fMicroSecs(0) };
};
