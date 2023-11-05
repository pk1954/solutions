// Win32_RootWindow.cpp
//
// Toolbox Win32_utilities

module;

#include <bit>
#include <string>
#include <memory>
#include <cassert>
#include <vector>
#include <chrono>
#include <Windows.h>
#include "CommCtrl.h"

module RootWindow;

import Types;
import Preferences;
import Win32_Util_Resource;
import BaseRefreshRate;
import WinManager;

using std::vector;
using std::wstring;
using std::bit_cast;
using std::make_unique;
using std::chrono::milliseconds;
using namespace std::chrono;

RootWindow * GetRootWindow(HWND const hwnd)
{
	auto pRootWin = bit_cast<RootWindow *>(::GetUserDataPtr(hwnd));
	return (pRootWin && pRootWin->GetWindowHandle())
		? pRootWin
		: nullptr;
}

RootWindow * GetParentRootWindow(HWND const hwnd)
{
	HWND hwndParent { GetParent(hwnd) };
	return hwndParent
		   ? GetRootWindow(hwndParent)
		   : nullptr;
}

wstring RootWindow::GetWindowText() const
{
	wstring wstrResult;
	int     iLength { GetWindowTextLength() };
	vector<wchar_t> buffer(iLength + 1);
	if (::GetWindowText(m_hwnd, buffer.data(), iLength) )
		wstrResult = wstring(buffer.data(), iLength);
	return wstrResult;
}

class RootWindow::WindowRefreshRate : public BaseRefreshRate
{
public:
	explicit WindowRefreshRate(RootWindow * const pRootWin)
		: m_pRootWin(pRootWin)
	{ }

	void Trigger(bool const bImmediately= false ) final
	{
		m_pRootWin->Trigger(bImmediately);
	}

private:
	RootWindow * const m_pRootWin;
};

RootWindow::RootWindow()
{
	m_upRefreshRate = make_unique<WindowRefreshRate>(this);
}

RootWindow::~RootWindow() 
{ 
	DestroyWindow();
}

void RootWindow::StartRootWindow(VisCrit const &visibilityCriterion)
{
	m_visibilityCriterion = visibilityCriterion;

	if (m_visibilityCriterion)
		m_visibilityMode = IsWindowVisible() ? tOnOffAuto::on : tOnOffAuto::off;
	else
		m_visibilityMode = tOnOffAuto::automatic;
}

void RootWindow::addWinMenu(HMENU const hMenuParent, wstring const & strTitle) const
{
	UINT  const STD_FLAGS = MF_BYPOSITION | MF_STRING;
	HMENU const hMenu = CreatePopupMenu();
	(void)AppendMenu(hMenu, STD_FLAGS, IDM_WINDOW_AUTO, L"auto");
	(void)AppendMenu(hMenu, STD_FLAGS, IDM_WINDOW_ON,   L"on" );
	(void)AppendMenu(hMenu, STD_FLAGS, IDM_WINDOW_OFF,  L"off");
	(void)AppendMenu(hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, strTitle.c_str());
}

void RootWindow::adjustWinMenu(HMENU const hMenu) const
{
	EnableMenuItem(hMenu, IDM_WINDOW_AUTO, ((m_visibilityMode == tOnOffAuto::automatic) ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, IDM_WINDOW_ON,   ((m_visibilityMode == tOnOffAuto::on       ) ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, IDM_WINDOW_OFF,  ((m_visibilityMode == tOnOffAuto::off      ) ? MF_GRAYED : MF_ENABLED));
}

void RootWindow::CenterIn(HWND const hwnd, PIXEL const width, PIXEL const height) const
{
	PixelRect rect { ::GetWindowRect(hwnd) };
	PIXEL     xPos { (rect.GetRight () + rect.GetLeft() - width ) / 2 };
	PIXEL     yPos { (rect.GetBottom() + rect.GetTop () - height) / 2 };
	Move(xPos, yPos, width, height, true);
}

void RootWindow::SetWindowHandle(HWND const hwnd) 
{ 
	assert(hwnd);
	m_hwnd = hwnd;  
};

void RootWindow::SetRefreshRate(milliseconds const msRate) const
{ 
	m_upRefreshRate->SetRefreshRate(msRate); 
}

void RootWindow::Notify(bool const bImmediately)
{
	m_upRefreshRate->Notify(bImmediately);
}

void RootWindow::DestroyWindow()
{
	::DestroyWindow(m_hwnd);
	m_hwnd = nullptr;
}

HBITMAP RootWindow::CreateCompatibleBitmap(HDC const hDC) const
{
	PixelRectSize const rectSize = GetClRectSize();
	return ::CreateCompatibleBitmap(hDC, rectSize.GetXvalue(), rectSize.GetYvalue());
}

bool RootWindow::IsOutOfClientRect(PixelPoint const &pnt) const
{
	PixelRect rect    { GetClPixelRect() };
	PIXEL     pixDist { rect.DistFromRect(pnt) };
	return pixDist > 0_PIXEL;
}

void RootWindow::SetTrackBarRange(INT const idTrackbar, LONG const lMin, LONG const lMax) const
{
	SendDlgItemMessage
	(
		idTrackbar, 
		TBM_SETRANGEMIN, 
		static_cast<WPARAM>(false),  // redraw flag 
		static_cast<LPARAM>(lMin) 
	);
	SendDlgItemMessage
	(
		idTrackbar, 
		TBM_SETRANGEMAX, 
		static_cast<WPARAM>(true),  // redraw flag 
		static_cast<LPARAM>(lMax) 
	);
}

void RootWindow::SetWindowVisibility(tOnOffAuto const mode)
{
	m_visibilityMode = mode;
	Show(ApplyAutoCriterion(m_visibilityMode, m_visibilityCriterion));
}

void RootWindow::colorDialog()
{
	static COLORREF acrCustClr[16]; // array of custom colors 
	CHOOSECOLOR cc;                 // common dialog box structure 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize  = sizeof(cc);
	cc.hwndOwner    = m_hwnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult    = GetBackgroundColorRef();
	cc.Flags        = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		SetBackgroundColorRef(cc.rgbResult);
		Trigger();
		WinManager::SendCommand2App(IDM_APP_DATA_CHANGED);
	}
}

void RootWindow::Move(PIXEL const xPos, PIXEL const yPos, PIXEL const width, PIXEL const height, bool const bRedraw) const
{
	::MoveWindow(m_hwnd, xPos.GetValue(), yPos.GetValue(), width.GetValue(), height.GetValue(), bRedraw);
}

void RootWindow::Move(PixelPoint const pos, PixelRectSize const size, bool const bRedraw) const
{
	Move(pos.GetX(), pos.GetY(), size.GetX(), size.GetY(), bRedraw);
}

void RootWindow::UpdateImmediately() const
{
	Invalidate(false);
	::UpdateWindow(m_hwnd);
}

void RootWindow::Invalidate(bool const bRedraw) const
{
	if (m_hwnd != nullptr)
		::InvalidateRect(m_hwnd, nullptr, bRedraw);
}

void RootWindow::Trigger(bool const bImmediately)
{
	if (bImmediately)
		UpdateImmediately();
	else
		Invalidate(false);
}
void RootWindow::FillBackground(HDC const hDC, COLORREF const col) const
{
	SetBkColor(hDC, col);
	::FastFill(hDC, m_hwnd);
}

void RootWindow::AddColorCtlMenu(HMENU const hPopupMenu)
{
	if (Preferences::m_bColorMenu.Get())
	{
		AppendMenu(hPopupMenu, MF_STRING, IDD_COLOR_CTL,        L"Choose background color");
		AppendMenu(hPopupMenu, MF_STRING, IDM_DEFAULT_BK_COLOR, L"Default background color");
	}
}

void RootWindow::OnContextMenu(WPARAM const wParam, LPARAM const lParam) // crsr pos in screen coordinates
{
	PixelPoint const& crsrPosScreen { GetCrsrPosFromLparam(lParam) };
	HMENU      const  hPopupMenu    { CreatePopupMenu() };

	LPARAM lParamNew = AddContextMenuEntries(hPopupMenu); // arbitrary parameter, forwarded as lParam  

	if (m_bParentContextMenue)
	{
		if (RootWindow * pParentRootWin { GetParentRootWindow() })
			pParentRootWin->AddContextMenuEntries(hPopupMenu);
	}

	if (m_visibilityCriterion)
	{
		addWinMenu(hPopupMenu, L"Show window");
		adjustWinMenu(hPopupMenu);
	}

	// TODO
	//if (m_bShowRefreshRateDlg && (m_upRefreshRate->GetRefreshRate() > 0ms))
	//{
	//	(void)AppendMenu(hPopupMenu, MF_STRING, IDD_REFRESH_RATE_DIALOG, L"Window refresh rate");
	//}

	(void)SetForegroundWindow(GetWindowHandle());

	auto const uiID = static_cast<UINT>
	(
		TrackPopupMenu
		(
			hPopupMenu,
			TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD | TPM_NONOTIFY,
			crsrPosScreen.GetXvalue(), crsrPosScreen.GetYvalue(),
			0,
			GetWindowHandle(),
			nullptr
		)
	);

	DestroyMenu(hPopupMenu);

	if (uiID != 0)
		OnCommand(uiID, lParamNew, Screen2Client(crsrPosScreen));
}

bool RootWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (UINT const uiCmdId { LOWORD(wParam) })
	{
	case IDM_WINDOW_ON:
		SetWindowVisibility(tOnOffAuto::on);
		break;

	case IDM_WINDOW_OFF:
		SetWindowVisibility(tOnOffAuto::off);
		break;

	case IDM_WINDOW_AUTO:
		SetWindowVisibility(tOnOffAuto::automatic);
		break;

	case IDD_REFRESH_RATE_DIALOG:
		m_upRefreshRate->RefreshRateDialog(m_hwnd);
		break;

	case IDD_SOUND:
		Preferences::m_bSound.Toggle();
		break;

	case IDD_COLOR_MENU:
		Preferences::m_bColorMenu.Toggle();
		break;

	case IDD_COLOR_CTL:
		colorDialog();
		break;

	case IDM_DEFAULT_BK_COLOR:
		SetDefaultBackgroundColor();
		Trigger();
		WinManager::SendCommand2App(IDM_APP_DATA_CHANGED);
		break;

	default:
		if (RootWindow * pParentRootWin { GetParentRootWindow() })
			return pParentRootWin->OnCommand(wParam, lParam, pixPoint);
		else
			return false;
	}

	return true;
}

bool RootWindow::OnMenuCommand(UINT const uiIndex, HMENU const hMenu)
{
	UINT_PTR data { ::GetMenuItemData(hMenu, uiIndex) };
	return OnCommand(data, uiIndex);
}

void RootWindow::OnClose()
{
	SendMessage(WM_COMMAND, IDM_WINDOW_OFF, 0);
}

bool RootWindow::OnSize(PIXEL const, PIXEL const)
{
	return false;
}

bool RootWindow::OnMove(PIXEL const, PIXEL const)
{
	return false;
}

// CommonMessageHandler
// used by dialogs and normal windows 

bool RootWindow::CommonMessageHandler(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
	switch (message)
	{

	case WM_CONTEXTMENU:
		OnContextMenu(wParam, lParam);
		return true;

	case WM_COMMAND:
		if (OnCommand(wParam, lParam))
			return true;
		break;

	case WM_MENUCOMMAND:
		return OnMenuCommand(static_cast<UINT>(wParam), bit_cast<HMENU>(lParam));
		break;

	case WM_CLOSE:
		OnClose();
		return true;

	case WM_SIZE:
		if (OnSize(static_cast<PIXEL>(LOWORD(lParam)), static_cast<PIXEL>(HIWORD(lParam))))
			return true;
		break;

	case WM_MOVE:
		if (OnMove(static_cast<PIXEL>(LOWORD(lParam)), static_cast<PIXEL>(HIWORD(lParam))))
			return true;
		break;

	default:
		break;
	}

	return false;
}
