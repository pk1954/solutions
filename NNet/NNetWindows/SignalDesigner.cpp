// SignalDesigner.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "PointType.h"
#include "NNetParameters.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "ComputeThread.h"
#include "SignalDesigner.h"

SignalDesigner::SignalDesigner
(
	HWND          const   hwndParent,
	ComputeThread const & computeThread,
	SignalGenerator     & sigGen
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		L"ClassSigDesWindow",
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SIZEBOX,
		nullptr,
		nullptr
	);

	m_graphics.Initialize(hwnd);
	SetWindowText(hwnd, L"SignalDesigner");
	m_trackStruct.hwndTrack = hwnd;

	m_upSignalControl = make_unique<SignalControl>
		                (
							hwnd, 
							computeThread, 
							sigGen, 
							&m_horzCoord, 
							&m_vertCoord
						);

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(100._MicroSecs, 1000000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);
	m_horzCoord.RegisterObserver(this);

	m_horzScale.InitHorzScale(&m_horzCoord, &m_graphics, L"s", 1e6f);
	m_horzScale.SetOrientation(false);
	m_horzScale.Recalc();

	m_vertCoord.SetPixelSize(0.25_fHertz);
	m_vertCoord.SetPixelSizeLimits(0.05_fHertz, 1._fHertz); 
	m_vertCoord.SetZoomFactor(1.3f);
	m_vertCoord.RegisterObserver(this);

	m_vertScale.InitVertScale(&m_vertCoord, &m_graphics, L"Hz", 1e0f);	
	m_vertScale.SetOrientation(true);
	m_vertScale.Recalc();

	::MoveWindow(hwnd, 100, 100, 500, 400, false);
	Util::Show(hwnd, true);
}

void SignalDesigner::Reset()
{
	m_trackStruct.hwndTrack = HWND(0);
	(void)TrackMouseEvent(& m_trackStruct);
}

void SignalDesigner::Stop()
{
	Reset();
	m_graphics.ShutDown();
	DestroyWindow();
}

void SignalDesigner::doPaint() const
{
	//fPixelRect fPixRect(m_fPixLeft, m_fPixTop, m_fPixRight, m_fPixBottom);
	//m_graphics.DrawRectangle(fPixRect, D2D1::ColorF::Black, 1.0_fPixel);
	
	m_upSignalControl->Invalidate(false);

	if (m_zoomMode == tZoomMode::HORZ)
		m_graphics.FillRectangle(m_horzScale.GetScaleRect(), D2D1::ColorF::Aquamarine);
	else if (m_zoomMode == tZoomMode::VERT)
		m_graphics.FillRectangle(m_vertScale.GetScaleRect(), D2D1::ColorF::Aquamarine);

	m_horzScale.Display();
	m_vertScale.Display();
}

void SignalDesigner::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint(&ps);
		if (m_graphics.StartFrame(hDC))
		{
			doPaint();
			m_graphics.EndFrame();
		}
		EndPaint(&ps);
	}
}

bool SignalDesigner::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	PIXEL width  = static_cast<PIXEL>(LOWORD(lParam));
	PIXEL height = static_cast<PIXEL>(HIWORD(lParam));

	m_graphics.Resize(width.GetValue(), height.GetValue());

	static fPixel const fPixLeftOffset   { Convert2fPixel(LEFT_OFFSET  ) };
	static fPixel const fPixBottomOffset { Convert2fPixel(BOTTOM_OFFSET) };
	static fPixel const fPixRightOffset  { Convert2fPixel(RIGHT_OFFSET ) };
	static fPixel const fPixTopOffset    { Convert2fPixel(TOP_OFFSET   ) };

	fPixel fPixWinWidth  { Convert2fPixel(PIXEL(width )) };
	fPixel fPixWinHeight { Convert2fPixel(PIXEL(height)) };

	m_horzCoord.SetOffset(fPixLeftOffset);
	m_vertCoord.SetOffset(fPixBottomOffset);
	m_horzScale.SetOrthoOffset(fPixBottomOffset);
	m_vertScale.SetOrthoOffset(fPixRightOffset);
	m_horzScale.Recalc();
	m_vertScale.Recalc();

	::MoveWindow
	(
		m_upSignalControl->GetWindowHandle(),
		LEFT_OFFSET.GetValue(),
		TOP_OFFSET.GetValue(),
		(width  - LEFT_OFFSET - RIGHT_OFFSET ).GetValue(),
		(height - TOP_OFFSET  - BOTTOM_OFFSET).GetValue(),
		true
	);

	Trigger();  // cause repaint
	return true;
}

bool SignalDesigner::horzScaleSelected(fPixelPoint const & pos) const
{
	return m_horzScale.GetScaleRect().Includes(pos);
}

bool SignalDesigner::vertScaleSelected(fPixelPoint const & pos) const
{
	return m_vertScale.GetScaleRect().Includes(pos);
}

void SignalDesigner::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };

	if (wParam & MK_LBUTTON)
	{
	}
	else  // left button not pressed: select
	{
		if (horzScaleSelected(fPixCrsrPos))
			m_zoomMode = tZoomMode::HORZ;
		else if (vertScaleSelected(fPixCrsrPos))
			m_zoomMode = tZoomMode::VERT;
		else
			m_zoomMode = tZoomMode::NONE;
	}
	Trigger();   // cause repaint
	(void)TrackMouseEvent(& m_trackStruct);
}

bool SignalDesigner::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	//WORD const wId = LOWORD(wParam);
	//switch (wId)
	//{
	////case IDD_APPLY_PARAMETERS:
	////	m_signalGenerator.SetParameterValues();
	////	return true;

	////case IDD_RESET_PARAMETERS:
	////	m_signalGenerator.LoadParameterValues();
	////	return true;

	//default:
	//	break;
	//}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

void SignalDesigner::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	if (m_zoomMode == tZoomMode::NONE)
		return;

	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
	{
		if (m_zoomMode == tZoomMode::HORZ)
			bResult = m_horzCoord.Zoom(bDirection);
		else if (m_zoomMode == tZoomMode::VERT)
			bResult = m_vertCoord.Zoom(bDirection);
	}
	if (!bResult)
		MessageBeep(MB_ICONWARNING);
}
