// NNetViewerWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <memory>
#include <algorithm>
#include <Windows.h>

module NNetViewerWindow;

import Win32_Util_Resource;
import SaveCast;
import IoUtil;

using std::unique_ptr;
using std::wstring;
using std::min;

NNetViewerWindow::~NNetViewerWindow() = default;

void NNetViewerWindow::Start()
{
	HWND hwnd = StartBaseWindow
	(
		nullptr,
		CS_HREDRAW | CS_VREDRAW,
		L"ClassScanViewerWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	m_statusBar.Start(hwnd);
	Show(true);
}

bool NNetViewerWindow::UserProc
(
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	//switch (message)
	//{

	//default:
	//	break;
	//}

	return BaseWindow::UserProc(message, wParam, lParam);
}

void NNetViewerWindow::arrangePanels(PixelRectSize const& pixWinSize)
{
	long const nrOfPanels { Cast2Long(m_panelList.size()) };
	if (nrOfPanels == 0)
		return;
	float const fRatioPanel   { m_panelList.front()->AspectRatio() };
	PIXEL       pixPanelWidth { 0_PIXEL };

	long nrOfRows { 1 };
	while (nrOfRows <= nrOfPanels)
	{
		long   const nrOfCols         { nrOfPanels / nrOfRows };
		PIXEL  const pixPanelWidth1   { pixWinSize.GetX() / nrOfCols };
		PIXEL  const pixPanelHeight   { pixWinSize.GetY() / nrOfRows };
		fPixel const fPixPanelWidth2  { Convert2fPixel(pixPanelHeight) / fRatioPanel };
		PIXEL  const pixPanelWidth2   { Convert2PIXEL(fPixPanelWidth2) };
		PIXEL  const pixPanelWidthMin { min(pixPanelWidth1, pixPanelWidth2) };
		if (pixPanelWidthMin > pixPanelWidth)
			pixPanelWidth = pixPanelWidthMin;
		else
			break;
		++nrOfRows;
	}
	fPixel        const fPixPanelHeight { Cast2Float(pixPanelWidth.GetValue()) * fRatioPanel };
	PIXEL         const pixPanelHeight  { Convert2PIXEL(fPixPanelHeight) };
	PixelRectSize const pixPanelSize(pixPanelWidth, pixPanelHeight);
	PIXEL               pixPanelPosX(0_PIXEL);
	PIXEL               pixPanelPosY(0_PIXEL);
	for (unique_ptr<ScanPanel>const &upPanel : m_panelList)
	{
		PixelPoint const pixPanelPos(pixPanelPosX, pixPanelPosY);
		PixelRect  const rect(pixPanelPos, pixPanelSize);
		upPanel->Move(rect, false);
		if (pixPanelPosX += pixPanelWidth > pixWinSize.GetX())
		{
			pixPanelPosX = 0_PIXEL;
			pixPanelPosY += pixPanelHeight; 
		}
	}
}

bool NNetViewerWindow::OnSize(PIXEL const width, PIXEL const height)
{
	arrangePanels(PixelRectSize(width, height));
	return true;
}

void NNetViewerWindow::OnClose()
{
	Stop();
}

void NNetViewerWindow::OnNotify(WPARAM const wParam, LPARAM const lParam)
{
}

void NNetViewerWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
}

bool NNetViewerWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	switch (wmId)
	{
	case IDX_FILE_NOT_FOUND:  //no user command, only internal usage
		MessageBox(nullptr, L"Could not find scan file", L"Error", MB_OK);
		return true;

	case IDX_ERROR_IN_FILE:  //no user command, only internal usage
		MessageBox(nullptr, L"Error reading scan file\r\nSee main_trace.out for details", L"Error", MB_OK);
		return true;

	default:
		return true;
	}

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

