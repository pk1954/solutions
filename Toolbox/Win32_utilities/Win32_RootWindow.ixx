// Win32_RootWindow.ixx
//
// Win32_utilities

module;

#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <vector>
#include <Windows.h>
#include <windowsx.h>
#include "CommCtrl.h"

export module RootWindow;

import SaveCast;
import BoolOp;
import Util;
import Types;
import ObserverInterface;
import Tooltip;
import Win32_Util;
import Win32_PIXEL;

using std::chrono::milliseconds;
using std::wostringstream;
using std::unique_ptr;
using std::wstring;
using std::vector;

class RootWindow;

export RootWindow* GetRootWindow(HWND const);
export RootWindow* GetParentRootWindow(HWND const);

export class RootWindow : public ObserverInterface
{
public:

	RootWindow();
	virtual ~RootWindow() override;

	void StartRootWindow(VisCrit const&);

	void Show(tBoolOp const op) const { ::Show(m_hwnd, op); }
	void Show(bool    const b)  const { ::Show(m_hwnd, b); }

	HDC  BeginPaint(LPPAINTSTRUCT lpPaint) const { return ::BeginPaint(m_hwnd, lpPaint); }
	bool EndPaint  (LPPAINTSTRUCT lpPaint) const { return ::EndPaint(m_hwnd, lpPaint); }

	HWND GetWindowHandle() const { return m_hwnd; };

	RootWindow * GetRootWindow      () { return ::GetRootWindow(m_hwnd); }
	RootWindow * GetParentRootWindow() { return ::GetParentRootWindow(m_hwnd); }

	RootWindow const * GetRootWindow      () const { return ::GetRootWindow(m_hwnd); }
	RootWindow const * GetParentRootWindow() const { return ::GetParentRootWindow(m_hwnd); }

	PIXEL         GetWindowTop()                      const { return ::GetWindowTop(m_hwnd); }
	PIXEL         GetWindowLeftPos()                  const { return ::GetWindowLeftPos(m_hwnd); }
	PIXEL         GetWindowRightPos()                 const { return ::GetWindowRightPos(m_hwnd); }
	PIXEL         GetWindowWidth()                    const { return ::GetWindowWidth(m_hwnd); }
	PIXEL         GetWindowHeight()                   const { return ::GetWindowHeight(m_hwnd); }
	PixelRectSize GetWindowSize()                     const { return ::GetWindowSize(m_hwnd); }
	PixelRectSize GetClRectSize()                     const { return ::GetClRectSize(m_hwnd); }
	PixelRect     GetClPixelRect()                    const { return ::GetClPixelRect(m_hwnd); }
	PIXEL         GetClientWindowHeight()             const { return ::GetClientWindowHeight(m_hwnd); }
	PIXEL         GetClientWindowWidth()              const { return ::GetClientWindowWidth(m_hwnd); }
	bool          CrsrInClientRect()                  const { return ::CrsrInClientRect(m_hwnd); }
	PixelPoint    GetRelativeCrsrPosition()           const { return ::GetRelativeCrsrPosition(m_hwnd); }
	PixelPoint    GetClRectCenter()                   const { return ::GetClRectCenter(m_hwnd); }
	bool          CrsrOutOfClientRect()               const { return IsOutOfClientRect(GetRelativeCrsrPosition()); }
	PixelPoint    Client2Screen(PixelPoint  const& p) const { return ::Client2Screen(m_hwnd, p); }
	PixelPoint    Screen2Client(PixelPoint  const& p) const { return ::Screen2Client(m_hwnd, p); }
	bool          IsInClientRect(PixelPoint const& p) const { return ::IsInClientRect(m_hwnd, p); }
	bool          IsInClientRect(PixelRect  const& r) const { return ::IsInClientRect(m_hwnd, r); }

	void  SetWindowWidth (PIXEL const w, bool const b)                         const { ::SetWindowWidth (m_hwnd, w, b); }
	void  SetWindowHeight(PIXEL const h, bool const b)                         const { ::SetWindowHeight(m_hwnd, h, b); }
	void  SetWindowSize  (PIXEL const w, PIXEL const h, bool const b)          const { ::SetWindowSize(m_hwnd, w, h, b); }
	bool  WindowHasCaption()                                                   const { return ::GetWindowLong(m_hwnd, GWL_STYLE) & WS_CAPTION; }
	bool  IsWindowVisible()                                                    const { return ::IsWindowVisible(m_hwnd); }
	HWND  SetCapture()                                                         const { return ::SetCapture(m_hwnd); }
	HWND  SetFocus()                                                           const { return ::SetFocus(m_hwnd); }
	HWND  GetParent()                                                          const { return ::GetParent(m_hwnd); }
	HWND  GetDlgItem(int const i)                                              const { return ::GetDlgItem(m_hwnd, i); }
	bool  IsCaptured()                                                         const { return ::GetCapture() == m_hwnd; }
	int	  GetWindowTextLength()                                                const { return ::GetWindowTextLength(m_hwnd); }
	DWORD SetWindowStyle(DWORD dwNew)                                          const { return   SetWindowLong(m_hwnd, GWL_STYLE, dwNew); }
	bool  SetWindowText(LPCWSTR        const   s)                              const { return ::SetWindowText(m_hwnd, s); }
	bool  SetWindowText(wstring        const & s)                              const { return   SetWindowText(s.c_str()); }
	bool  SetWindowText(wostringstream const & s)                              const { return   SetWindowText(s.str()); }
	bool  SetWindowText(HWND hwnd, wstring        const & s)                   const { return ::SetWindowText(hwnd, s.c_str()); }
	bool  SetWindowText(HWND hwnd, wostringstream const & s)                   const { return   SetWindowText(hwnd, s.str()); }
	void  CheckRadioButton(int iFirst, int iLast, int iCheck)                  const { ::CheckRadioButton(m_hwnd, iFirst, iLast, iCheck); }
	UINT  IsDlgButtonChecked(int iIdButton)                                    const { return ::IsDlgButtonChecked(m_hwnd, iIdButton); }
	bool  Move(PIXEL const xPos, PIXEL const yPos)                             const { return ::MoveWindow(m_hwnd, xPos, yPos); }
	bool  Move(PixelPoint const pos)                                           const { return ::MoveWindow(m_hwnd, pos); }
	void  Move(PixelRect const rect, bool const bRedraw)                       const { Move(rect.GetStartPoint(), rect.GetSize(), bRedraw); }
	HWND  CreateBalloonToolTip(int const id, LPWSTR const& t)                  const { return ::CreateBalloonToolTip(m_hwnd, id, t); }
	HWND  CreateStdToolTip(int const id, LPWSTR const& t)                      const { return ::CreateStdToolTip(m_hwnd, id, t); }
	HWND  CreateWindowToolTip(LPWSTR const& t)                                 const { return ::CreateWindowToolTip(m_hwnd, t); }
	HWND  CreateRectToolTip(int const id, PixelRect const& r, LPWSTR const& t) const { return ::CreateRectToolTip(m_hwnd, id, &r, t); }

	LRESULT PostCommand2Application(                WPARAM const wParam, LPARAM const lParam = 0) const { return ::PostMessage(m_hwndApp, WM_COMMAND, wParam, lParam); }
	LRESULT SendCommand2Application(                WPARAM const wParam, LPARAM const lParam = 0) const { return ::SendMessage(m_hwndApp, WM_COMMAND, wParam, lParam); }
	LRESULT SendNotifyCommand      (                WPARAM const wParam, LPARAM const lParam = 0) const { return   SendNotifyMessage(     WM_COMMAND, wParam, lParam); }
	LRESULT PostCommand            (                WPARAM const wParam, LPARAM const lParam = 0) const { return   PostMessage(           WM_COMMAND, wParam, lParam); }
	LRESULT SendCommand            (                WPARAM const wParam, LPARAM const lParam = 0) const { return   SendMessage(           WM_COMMAND, wParam, lParam); }
	LRESULT SendCommand2Parent     (                WPARAM const wParam, LPARAM const lParam = 0) const { return   SendMessage2Parent(    WM_COMMAND, wParam, lParam); }
	LRESULT PostCommand2Parent     (                WPARAM const wParam, LPARAM const lParam = 0) const { return   PostMessage2Parent(    WM_COMMAND, wParam, lParam); }
	LRESULT PostMessage            (UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const	{ return ::PostMessage(m_hwnd,       msg,     wParam, lParam); }
	LRESULT PostMessage2Parent     (UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const { return ::PostMessage(GetParent(),  msg,     wParam, lParam); }
	LRESULT SendMessage            (UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const	{ return ::SendMessage(m_hwnd,       msg,     wParam, lParam); }
	LRESULT SendMessage2Parent     (UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const { return ::SendMessage(GetParent(),  msg,     wParam, lParam); }
	LRESULT SendMessage2Application(UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const	{ return ::SendMessage(m_hwndApp,    msg,     wParam, lParam); }
	LRESULT SendNotifyMessage      (UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const { return ::SendNotifyMessage(m_hwnd, msg,     wParam, lParam); }

	LRESULT const DefWindowProc    (UINT const msg, WPARAM const wParam, LPARAM const lParam) const { return ::DefWindowProc(m_hwnd, msg, wParam, lParam); }

	LRESULT SendDlgItemMessage(int const iItem, unsigned int msg, WPARAM wParam, LPARAM lParam) const { return ::SendDlgItemMessage(m_hwnd, iItem, msg, wParam, lParam); }

	void SetDlgText(int const iItem, wchar_t const* const wstrText) const {	::SetText(GetDlgItem(iItem), wstrText);	}

	short      GetTrackBarPos(INT const idTrackbar)      const { return Cast2Short(SendDlgItemMessage(idTrackbar, TBM_GETPOS, 0, 0)); }
	PixelPoint GetCrsrPosFromLparam(LPARAM const lParam) const { return PixelPoint { CrsrXpos(lParam), CrsrYpos(lParam) }; }

	PIXEL CrsrXpos(LPARAM const lParam) const { return PIXEL(GET_X_LPARAM(lParam)); }
	PIXEL CrsrYpos(LPARAM const lParam) const { return PIXEL(GET_Y_LPARAM(lParam)); }

	virtual LPARAM AddContextMenuEntries(HMENU const) { return 0L; }

	virtual void Trigger() { Invalidate(false); }
	virtual void Stop()	   { DestroyWindow(); }

	void ShowRefreshRateDlg(bool const bShow) { m_bShowRefreshRateDlg = bShow; }
	void SetWindowVisibility(tOnOffAuto const);

	void SetParentContextMenueMode(bool const b) { m_bParentContextMenue = b; }

	virtual void SetDefaultBackgroundColor() {};
	virtual void AddColorCtlMenu(HMENU const);

	virtual void     SetBackgroundColorRef(COLORREF const c) {}
	virtual COLORREF GetBackgroundColorRef() const           { return RGB(0, 0, 0); }

	virtual void    SetGrid  (bool const, bool const) {}
	virtual void    SetScales(bool const, bool const) {}

	virtual bool HasGrid()   const { return false; }
	virtual bool HasScales() const { return false; }

	void Notify(bool const) override;

	void    Move(PIXEL const, PIXEL const, PIXEL const, PIXEL const, bool const) const;
	void    Move(PixelPoint const, PixelRectSize const, bool const)              const;
	void    UpdateImmediately()                                                  const;
	void    Invalidate(bool const)                                               const;
	void    FillBackground(HDC const, COLORREF const)                            const;
	void    SetTrackBarRange(INT const, LONG const, LONG const)                  const;
	void    CenterIn(HWND const, PIXEL const, PIXEL const)                       const;
	HBITMAP CreateCompatibleBitmap(HDC const hDC)                                const;
	void    SetRefreshRate(milliseconds const)                                   const;
	bool    IsOutOfClientRect(PixelPoint const&)                                 const;
	wstring GetWindowText()                                                      const;
	void    DestroyWindow();

protected:

	void SetWindowHandle(HWND const);

	virtual bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL());
	virtual void OnContextMenu(WPARAM const, LPARAM const);
	virtual bool OnMenuCommand(UINT   const, HMENU  const);
	virtual bool OnSize(PIXEL const, PIXEL const);
	virtual bool OnMove(PIXEL const, PIXEL const);
	virtual void OnClose();

	virtual bool CommonMessageHandler(UINT const, WPARAM const, LPARAM const);

private:

	class WindowRefreshRate;
	unique_ptr<WindowRefreshRate> m_upRefreshRate;

	tOnOffAuto m_visibilityMode      { tOnOffAuto::on };
	VisCrit    m_visibilityCriterion { nullptr };
	bool       m_bShowRefreshRateDlg { true };
	HWND       m_hwndApp             { nullptr };
	HWND       m_hwnd                { nullptr };
	bool       m_bParentContextMenue { false };

	void addWinMenu(HMENU const, wstring const&) const;
	void adjustWinMenu(HMENU const) const;
	void colorDialog();

	void adjustVisibility(tOnOffAuto const onOffAuto) const
	{
		if (m_visibilityCriterion)
			Show(ApplyAutoCriterion(onOffAuto, m_visibilityCriterion));
	}
};
