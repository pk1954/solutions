// win32_rootWindow.cpp
//
// Toolbox Win32_utilities

#include "stdafx.h"
#include "PixelTypes.h"
#include "win32_util_resource.h"
#include "win32_baseRefreshRate.h"
#include "win32_rootWindow.h"

using std::bit_cast;
using std::make_unique;

RootWindow * GetRootWindow(HWND const hwnd)
{
	auto pRootWin = bit_cast<RootWindow *>(Util::GetUserDataPtr(hwnd));
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

	void Trigger() final
	{
		m_pRootWin->Trigger();
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
	EnableMenuItem(hMenu, IDM_WINDOW_ON,   ((m_visibilityMode == tOnOffAuto::on      ) ? MF_GRAYED : MF_ENABLED));
	EnableMenuItem(hMenu, IDM_WINDOW_OFF,  ((m_visibilityMode == tOnOffAuto::off     ) ? MF_GRAYED : MF_ENABLED));
}

void RootWindow::contextMenu(PixelPoint const & crsrPosScreen) // crsr pos in screen coordinates
{
	HMENU const hPopupMenu { CreatePopupMenu() };

	LPARAM lParam = AddContextMenuEntries(hPopupMenu); // arbitrary parameter, forwarded as lParam  

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
	//if (m_bShowRefreshRateDlg && (m_pRefreshRate->GetRefreshRate() > 0ms))
	//{
	//	(void)AppendMenu(hPopupMenu, MF_STRING, IDD_REFRESH_RATE_DIALOG, L"Window refresh rate");
	//}

	(void)SetForegroundWindow(GetWindowHandle());

	auto const uiID = static_cast<UINT>
	(
		TrackPopupMenu
	    (
			hPopupMenu, 
			TPM_TOPALIGN|TPM_LEFTALIGN|TPM_RETURNCMD|TPM_NONOTIFY, 
			crsrPosScreen.GetXvalue(), crsrPosScreen.GetYvalue(), 
			0, 
			GetWindowHandle(),
			nullptr 
	    )
	);

	DestroyMenu(hPopupMenu);

	if (uiID != 0)
		OnCommand(uiID, lParam, Screen2Client(crsrPosScreen));
}

void RootWindow::CenterIn(HWND const hwnd, PIXEL const width, PIXEL const height) const
{
	PixelRect rect { Util::GetWindowRect(hwnd) };
	PIXEL xPos { (rect.GetRight () + rect.GetLeft() - width ) / 2 };
	PIXEL yPos { (rect.GetBottom() + rect.GetTop () - height) / 2 };
	Move(xPos, yPos, width, height, true);
}

void RootWindow::SetWindowHandle(HWND const hwnd) 
{ 
	assert(hwnd);
	m_hwnd    = hwnd;  
	m_hwndApp = GetAncestor(m_hwnd, GA_ROOTOWNER);
};

void RootWindow::SetRefreshRate(milliseconds const msRate) const
{ 
	m_upRefreshRate->SetRefreshRate(msRate); 
}

void RootWindow::Notify(bool const bImmediately)
{
	m_upRefreshRate->Notify(bImmediately);
}

void RootWindow::SetTrackBarPos(INT const idTrackbar, LONG const lPos) const
{
	SendDlgItemMessage
	(  
		idTrackbar, TBM_SETPOS, 
		static_cast<WPARAM>(true),  // redraw flag 
		static_cast<LPARAM>(lPos)
	); 
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

bool RootWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (UINT const uiCmdId { LOWORD(wParam) } )
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
	UINT_PTR data { Util::GetMenuItemData(hMenu, uiIndex) };
	return OnCommand(data, uiIndex);
}

void RootWindow::OnClose()
{
	SendMessage(WM_COMMAND, IDM_WINDOW_OFF, 0);
}

bool RootWindow::OnSize(PIXEL const width, PIXEL const height)
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
		contextMenu(GetCrsrPosFromLparam(lParam));
		return true;

	case WM_COMMAND:
		if (OnCommand(wParam, lParam))
			return true;
		break;

	case WM_MENUCOMMAND:
	{
		return OnMenuCommand
		(
			static_cast<UINT>(wParam), 
			bit_cast<HMENU>(lParam)
		);
	}
		break;

	case WM_CLOSE:
		OnClose();
		return true;

	case WM_SIZE:
		if (OnSize(static_cast<PIXEL>(LOWORD(lParam)), static_cast<PIXEL>(HIWORD(lParam))))
			return true;
		break;

	default:
		break;
	}

	return false;
}
