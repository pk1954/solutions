// win32_rootWindow.h : 
//
// Win32_utilities

#pragma once

#include <chrono>
#include <vector>
#include "windowsx.h"
#include "CommCtrl.h"
#include "PixelTypes.h"
#include "observerInterface.h"
#include "win32_tooltip.h"
#include "win32_util.h"

using std::chrono::milliseconds;
using std::unique_ptr;
using std::function;
using std::vector;

inline LONG_PTR GetUserDataPtr(HWND hwnd)               { return GetWindowLongPtr(hwnd, GWLP_USERDATA); }
inline void     SetUserDataPtr(HWND hwnd, LONG_PTR ptr) { (void) SetWindowLongPtr(hwnd, GWLP_USERDATA, ptr); }

class RootWindow : public ObserverInterface
{
public:

    RootWindow();
    ~RootWindow() override;

	void StartRootWindow(function<bool()> const);

    HWND GetWindowHandle() const { return m_hwnd; };

    PIXEL         GetWindowTop()            const { return Util::GetWindowTop           (m_hwnd); }
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

	bool          IsWindowVisible()           const { return ::IsWindowVisible(m_hwnd); }
	HWND          SetCapture()                const { return ::SetCapture     (m_hwnd); }
    HWND          SetFocus()                  const { return ::SetFocus       (m_hwnd); }
    HWND          GetDlgItem(int const iItem) const { return ::GetDlgItem     (m_hwnd, iItem); }
    bool          IsCaptured()                const { return ::GetCapture() == m_hwnd; }
	int			  GetWindowTextLength()       const { return ::GetWindowTextLength(m_hwnd); }        

	void SetRefreshRate(milliseconds const) const;
	
    void Show(tBoolOp const op) const { Util::Show(m_hwnd, op); }
    void Show(bool    const b ) const { Util::Show(m_hwnd, b ); }

    HDC  BeginPaint(LPPAINTSTRUCT lpPaint) const { return ::BeginPaint(m_hwnd, lpPaint); }
    bool EndPaint  (LPPAINTSTRUCT lpPaint) const { return ::EndPaint  (m_hwnd, lpPaint); }

	void SetTrackBarPos(INT const, LONG const) const;
	void SetTrackBarRange(INT const, LONG const, LONG const) const;

	void DestroyWindow()
	{ 
		::DestroyWindow(m_hwnd);
		m_hwnd = nullptr;
	}

	HWND CreateBalloonToolTip(int const id, LPWSTR const t) const
	{
		return ::CreateBalloonToolTip(m_hwnd, id, t);	
	}

	HWND CreateStdToolTip(int const id, LPWSTR const t) const
	{ 
		return ::CreateStdToolTip(m_hwnd, id, t);	
	}

	HWND CreateWindowToolTip(LPWSTR const t) const
	{ 
		return ::CreateWindowToolTip(m_hwnd, t);	
	}

	HWND CreateRectToolTip(int const id, PixelRect * const pRect, LPWSTR const t) const
	{ 
		return ::CreateRectToolTip(m_hwnd, id, pRect, t);	
	}

	HBITMAP CreateCompatibleBitmap(HDC const hDC) const
	{
		PixelRectSize const rectSize = GetClRectSize();
		return ::CreateCompatibleBitmap(hDC, rectSize.GetXvalue(), rectSize.GetYvalue());
	}
	
	LRESULT const PostCommand(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return PostMessage(WM_COMMAND, wParam, lParam);
	}

	LRESULT const SendCommand(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return SendMessage(WM_COMMAND, wParam, lParam);
	}

	LRESULT const SendNotifyCommand(WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return SendNotifyMessage(WM_COMMAND, wParam, lParam);
	}

	LRESULT const PostMessage(UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return ::PostMessage(m_hwnd, msg, wParam, lParam);
	}

	LRESULT const SendMessage(UINT const msg, WPARAM const wParam, LPARAM const lParam = 0) const
	{
		return ::SendMessage(m_hwnd, msg, wParam, lParam);
	}

	LRESULT const SendNotifyMessage(UINT const msg, WPARAM const wParam, LPARAM const lParam) const
	{
		return ::SendNotifyMessage(m_hwnd, msg, wParam, lParam);
	}

	LRESULT const SendDlgItemMessage(int const iItem, UINT msg, WPARAM wParam, LPARAM lParam) const
    {
        return ::SendDlgItemMessage(m_hwnd, iItem, msg, wParam, lParam);
    }

	void SetDlgText(int const iItem, wchar_t const * const wstrText)
	{
		Util::SetText(GetDlgItem(iItem), wstrText);
	}

    LRESULT const DefWindowProc(UINT msg, WPARAM wParam, LPARAM lParam) const
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

	void Update() const
	{
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

	virtual void Trigger()	{ Invalidate(false); }

	void ShowRefreshRateDlg(bool const bShow) { m_bShowRefreshRateDlg = bShow; }
	void SetWindowVisibility(tOnOffAuto const);

protected:

	void SetWindowHandle(HWND const);

	virtual bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL());
	virtual bool OnSize   (WPARAM const, LPARAM const);
	virtual void OnClose();

	virtual bool CommonMessageHandler(UINT const, WPARAM const, LPARAM const);

private:

	class WindowRefreshRate;
	unique_ptr<WindowRefreshRate> m_upRefreshRate;

	HWND             m_hwnd                { nullptr };
	HWND             m_hwndApp             { nullptr };
	tOnOffAuto       m_visibilityMode      { tOnOffAuto::on };
	function<bool()> m_visibilityCriterion { nullptr };
	bool             m_bShowRefreshRateDlg { true };

	void addWinMenu(HMENU const, std::wstring const) const;
	void adjustWinMenu(HMENU const) const;
	void contextMenu(PixelPoint const &);

	void adjustVisibility(tOnOffAuto const onOffAuto) const
	{
		if (m_visibilityCriterion)
			Show(ApplyAutoCriterion(onOffAuto, m_visibilityCriterion));
	}

};

RootWindow * GetRootWindow(HWND const);
