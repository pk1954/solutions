// win32_rootWindow.h : 
//
// Win32_utilities

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <vector>
#include "windowsx.h"
#include "CommCtrl.h"
#include "win32_util.h"

import SaveCast;
import Util;
import ObserverInterface;
import Tooltip;
import PixelTypes;

using std::chrono::milliseconds;
using std::wostringstream;
using std::unique_ptr;
using std::wstring;
using std::vector;

class RootWindow;

RootWindow * GetRootWindow(HWND const);
RootWindow * GetParentRootWindow(HWND const);

class RootWindow : public ObserverInterface
{
public:

    RootWindow();
    ~RootWindow() override;

	void StartRootWindow(VisCrit const &);

    HWND GetWindowHandle() const { return m_hwnd; };

	PIXEL         GetWindowTop()            const { return Util::GetWindowTop           (m_hwnd); }
	PIXEL         GetWindowLeftPos()        const { return Util::GetWindowLeftPos       (m_hwnd); }
	PIXEL         GetWindowRightPos()       const { return Util::GetWindowRightPos      (m_hwnd); }
	PIXEL         GetWindowWidth()          const { return Util::GetWindowWidth         (m_hwnd); }
    PIXEL         GetWindowHeight()         const { return Util::GetWindowHeight        (m_hwnd); }
    PixelRectSize GetWindowSize()           const { return Util::GetWindowSize          (m_hwnd); }
    PixelRectSize GetClRectSize()           const { return Util::GetClRectSize          (m_hwnd); }
    PixelRect     GetClPixelRect()          const { return Util::GetClPixelRect         (m_hwnd); }
    PIXEL         GetClientWindowHeight()   const { return Util::GetClientWindowHeight  (m_hwnd); }
    PIXEL         GetClientWindowWidth()    const { return Util::GetClientWindowWidth   (m_hwnd); }
    bool          CrsrInClientRect()        const { return Util::CrsrInClientRect       (m_hwnd); }
    PixelPoint    GetRelativeCrsrPosition() const { return Util::GetRelativeCrsrPosition(m_hwnd); }
    PixelPoint    GetClRectCenter()         const { return Util::GetClRectCenter        (m_hwnd); }

	PixelPoint    Client2Screen(PixelPoint  const & p) const { return Util::Client2Screen (m_hwnd, p); }
	PixelPoint    Screen2Client(PixelPoint  const & p) const { return Util::Screen2Client (m_hwnd, p); }

	bool          IsInClientRect(PixelPoint const & p) const { return Util::IsInClientRect(m_hwnd, p); }
	bool          IsInClientRect(PixelRect  const & r) const { return Util::IsInClientRect(m_hwnd, r); }

	void SetWindowWidth (PIXEL const w, bool const b) const { Util::SetWindowWidth (m_hwnd, w, b); }
	void SetWindowHeight(PIXEL const h, bool const b) const { Util::SetWindowHeight(m_hwnd, h, b); }

	RootWindow * GetRootWindow      () { return ::GetRootWindow      (m_hwnd); }
	RootWindow * GetParentRootWindow() { return ::GetParentRootWindow(m_hwnd); }

	RootWindow const * GetRootWindow      () const { return ::GetRootWindow      (m_hwnd); }
	RootWindow const * GetParentRootWindow() const { return ::GetParentRootWindow(m_hwnd); }

	bool WindowHasCaption()      const { return ::GetWindowLong  (m_hwnd, GWL_STYLE) & WS_CAPTION; }
	bool IsWindowVisible()       const { return ::IsWindowVisible(m_hwnd); }
	HWND SetCapture()            const { return ::SetCapture     (m_hwnd); }
	HWND SetFocus()              const { return ::SetFocus       (m_hwnd); }
	HWND GetParent()             const { return ::GetParent      (m_hwnd); }
	HWND GetDlgItem(int const i) const { return ::GetDlgItem     (m_hwnd, i); }
	bool IsCaptured()            const { return ::GetCapture() == m_hwnd; }
	int	 GetWindowTextLength()   const { return ::GetWindowTextLength(m_hwnd); }    

	wstring GetWindowText() const; 

	bool SetWindowText(LPCWSTR        const   s) const { return ::SetWindowText(m_hwnd, s); }
	bool SetWindowText(wstring        const & s) const { return   SetWindowText(s.c_str()); }
	bool SetWindowText(wostringstream const & s) const { return   SetWindowText(s.str()); }

	bool SetWindowText(HWND hwnd, wstring        const & s) const { return ::SetWindowText(hwnd, s.c_str()); }
	bool SetWindowText(HWND hwnd, wostringstream const & s) const { return   SetWindowText(hwnd, s.str()); }

	void SetRefreshRate(milliseconds const) const;
	
    void Show(tBoolOp const op) const { Util::Show(m_hwnd, op); }
    void Show(bool    const b ) const { Util::Show(m_hwnd, b ); }

    HDC  BeginPaint(LPPAINTSTRUCT lpPaint) const { return ::BeginPaint(m_hwnd, lpPaint); }
    bool EndPaint  (LPPAINTSTRUCT lpPaint) const { return ::EndPaint  (m_hwnd, lpPaint); }

	void SetTrackBarPos(INT const, LONG const) const;
	void SetTrackBarRange(INT const, LONG const, LONG const) const;

	void CenterIn(HWND const, PIXEL const, PIXEL const) const;

	void DestroyWindow()
	{ 
		::DestroyWindow(m_hwnd);
		m_hwnd = nullptr;
	}

	HWND CreateBalloonToolTip(int const id, LPWSTR const &t) const
	{
		return ::CreateBalloonToolTip(m_hwnd, id, t);	
	}

	HWND CreateStdToolTip(int const id, LPWSTR const &t) const
	{ 
		return ::CreateStdToolTip(m_hwnd, id, t);	
	}

	HWND CreateWindowToolTip(LPWSTR const &t) const
	{ 
		return ::CreateWindowToolTip(m_hwnd, t);	
	}

	HWND CreateRectToolTip(int const id, PixelRect const & rect, LPWSTR const &t) const
	{ 
		return ::CreateRectToolTip(m_hwnd, id, &rect, t);
	}

	HBITMAP CreateCompatibleBitmap(HDC const hDC) const
	{
		PixelRectSize const rectSize = GetClRectSize();
		return ::CreateCompatibleBitmap(hDC, rectSize.GetXvalue(), rectSize.GetYvalue());
	}
	
	LRESULT PostCommand(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return PostMessage(WM_COMMAND, wParam, lParam);
	}

	LRESULT PostCommand2Parent(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return PostMessage2Parent(WM_COMMAND, wParam, lParam);
	}

	LRESULT SendCommand(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return SendMessage(WM_COMMAND, wParam, lParam);
	}

	LRESULT SendCommand2Parent(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return SendMessage2Parent(WM_COMMAND, wParam, lParam);
	}

	LRESULT SendNotifyCommand(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return SendNotifyMessage(WM_COMMAND, wParam, lParam);
	}

	LRESULT PostMessage(unsigned int const msg, WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return ::PostMessage(m_hwnd, msg, wParam, lParam);
	}

	LRESULT PostMessage2Parent(unsigned int const msg, WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return ::PostMessage(GetParent(), msg, wParam, lParam);
	}

	LRESULT SendMessage(unsigned int const msg, WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return ::SendMessage(m_hwnd, msg, wParam, lParam);
	}

	LRESULT SendMessage2Parent(unsigned int const msg, WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return ::SendMessage(GetParent(), msg, wParam, lParam);
	}

	LRESULT SendNotifyMessage(unsigned int const msg, WPARAM const wParam, LPARAM const lParam) const
	{
		return ::SendNotifyMessage(m_hwnd, msg, wParam, lParam);
	}

	LRESULT SendDlgItemMessage(int const iItem, unsigned int msg, WPARAM wParam, LPARAM lParam) const
    {
        return ::SendDlgItemMessage(m_hwnd, iItem, msg, wParam, lParam);
    }

	void SetDlgText(int const iItem, wchar_t const * const wstrText) const
	{
		Util::SetText(GetDlgItem(iItem), wstrText);
	}

    LRESULT const DefWindowProc(unsigned int msg, WPARAM wParam, LPARAM lParam) const
    {
        return ::DefWindowProc(m_hwnd, msg, wParam, lParam);
    }

	void CheckRadioButton(int iFirst, int iLast, int iCheck) const
	{
		(void)::CheckRadioButton(m_hwnd, iFirst, iLast, iCheck);
	}

	UINT IsDlgButtonChecked(int iIdButton) const
	{
		return ::IsDlgButtonChecked(m_hwnd, iIdButton);
	}

	void Move(PIXEL const xPos, PIXEL const yPos, PIXEL const width, PIXEL const height, bool const bRedraw) const
    {
        (void)::MoveWindow(m_hwnd, xPos.GetValue(), yPos.GetValue(), width.GetValue(), height.GetValue(), bRedraw);
    }

    void Move(PixelPoint const pos, PixelRectSize const size, bool const bRedraw) const
    {
        Move(pos.GetX(), pos.GetY(), size.GetX(), size.GetY(), bRedraw);
    }

    void Move(PixelRect const rect, bool const bRedraw) const
    {
        Move(rect.GetStartPoint(), rect.GetSize(), bRedraw);
    }

	void UpdateImmediately() const
	{
		Invalidate(false);
		(void)::UpdateWindow(m_hwnd);
	}

	void Invalidate(bool const bRedraw) const 
	{
		if (m_hwnd != nullptr)
			(void)::InvalidateRect(m_hwnd, nullptr, bRedraw);
	}

	void FillBackground(HDC const hDC, COLORREF const col) const
    {
        SetBkColor(hDC, col);
        Util::FastFill(hDC, m_hwnd);
    }

	short GetTrackBarPos(INT const idTrackbar) const
	{
		return Cast2Short(SendDlgItemMessage(idTrackbar, TBM_GETPOS, 0, 0)); 
	}

	LRESULT PostCommand2Application(WPARAM const wParam, LPARAM const lParam) const
	{
		return ::PostMessage(m_hwndApp, WM_COMMAND, wParam, lParam);
	}

	LRESULT SendCommand2Application(WPARAM const wParam, LPARAM const lParam) const
	{
		return ::SendMessage(m_hwndApp, WM_COMMAND, wParam, lParam);
	}

	PIXEL CrsrXpos(LPARAM const lParam) const
	{
		return PIXEL(GET_X_LPARAM(lParam));
	}

	PIXEL CrsrYpos(LPARAM const lParam) const
	{
		return PIXEL(GET_Y_LPARAM(lParam));
	}

	PixelPoint GetCrsrPosFromLparam(LPARAM const lParam) const
	{
		return PixelPoint{ CrsrXpos(lParam), CrsrYpos(lParam) };
	}

	virtual LPARAM AddContextMenuEntries(HMENU const) { return 0L; }

	void Notify(bool const) override;

	virtual void Trigger() { Invalidate(false); }

	void ShowRefreshRateDlg(bool const bShow) { m_bShowRefreshRateDlg = bShow; }
	void SetWindowVisibility(tOnOffAuto const);

	void SetParentContextMenueMode(bool const b) { m_bParentContextMenue = b; }

protected:

	void SetWindowHandle(HWND const);

	virtual bool OnCommand    (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL());
	virtual bool OnMenuCommand(UINT   const, HMENU  const);
	virtual bool OnSize(PIXEL const, PIXEL const);
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

	void addWinMenu(HMENU const, wstring const &) const;
	void adjustWinMenu(HMENU const) const;
	void contextMenu(PixelPoint const &);

	void adjustVisibility(tOnOffAuto const onOffAuto) const
	{
		if (m_visibilityCriterion)
			Show(ApplyAutoCriterion(onOffAuto, m_visibilityCriterion));
	}
};
