// win32_baseWindow.h : 
//
// win32_utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"
#include "win32_hiResTimer.h"
#include "win32_rootWindow.h"

static LRESULT CALLBACK BaseWndProc(HWND const, UINT const, WPARAM const, LPARAM const);

class BaseWindow : public RootWindow
{
public:

    HWND StartBaseWindow
	(
		HWND      const, 
		UINT      const, 
		LPCTSTR   const, 
		DWORD     const, 
		PixelRect const *,
		VisCrit   const &
	);

	void SetCaption() const;
	
	virtual wstring GetCaption() const;

	static void SetPerfMonMode(bool const b) { m_bPerfMonMode = b; }
	static bool PerfMonMode() { return m_bPerfMonMode; }

protected:

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

	virtual void OnPaint          ()                           { /* empty */ };
	virtual void OnLButtonDblClick(WPARAM const, LPARAM const) { /* empty */ };
	virtual void OnMouseWheel     (WPARAM const, LPARAM const) { /* empty */ };
	virtual void OnChar           (WPARAM const, LPARAM const) { /* empty */ };
	virtual void OnNotify         (WPARAM const, LPARAM const) { /* empty */ };
	virtual void OnMove           (WPARAM const, LPARAM const) { /* empty */ };
	virtual bool OnRButtonDown    (WPARAM const, LPARAM const) { return false; };
	virtual bool OnRButtonUp      (WPARAM const, LPARAM const) { return false; };
	virtual bool OnLButtonDown    (WPARAM const, LPARAM const) { return false; };
	virtual bool OnLButtonUp      (WPARAM const, LPARAM const) { return false; };
	virtual bool OnShow           (WPARAM const, LPARAM const) { return false; };
	virtual bool OnSetCursor      (WPARAM const, LPARAM const) { return false; };
	virtual bool OnTimer          (WPARAM const, LPARAM const) { return false; };
	virtual void OnMouseHover     (WPARAM const, LPARAM const);
	virtual void OnMouseMove      (WPARAM const, LPARAM const);
	virtual void OnMouseLeave     ();

	bool OnSize(PIXEL const, PIXEL const) override { return false; };

private:

	void trackMouse(bool const);

	inline static bool m_bPerfMonMode { false };

	friend static LRESULT CALLBACK BaseWndProc(HWND const, UINT const, WPARAM const, LPARAM const);

	HiResTimer m_paintTimer;
	fMicroSecs m_usPaintTime { 0._MicroSecs };
};
