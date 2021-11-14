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
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SIZEBOX | WS_VISIBLE,
		nullptr,
		nullptr
	);

	m_graphics.Initialize(hwnd);
	SetWindowText(hwnd, L"SignalDesigner");

	m_upSignalControl = make_unique<SignalControl>
	(
		hwnd, 
		computeThread, 
		sigGen, 
		&m_horzCoord, 
		&m_vertCoord
	);
	m_upHorzScale = make_unique<Scale<fMicroSecs>>(hwnd);
	m_upVertScale = make_unique<Scale<fHertz    >>(hwnd);

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(100._MicroSecs, 1000000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);
	m_horzCoord.RegisterObserver(this);

	m_upHorzScale->InitHorzScale(&m_horzCoord, &m_graphics, L"s", 1e6f);
	m_upHorzScale->SetOrientation(false);
	m_upHorzScale->Notify(false);

	m_vertCoord.SetPixelSize(0.25_fHertz);
	m_vertCoord.SetPixelSizeLimits(0.05_fHertz, 1._fHertz); 
	m_vertCoord.SetZoomFactor(1.3f);
	m_vertCoord.RegisterObserver(this);

	m_upVertScale->InitVertScale(&m_vertCoord, &m_graphics, L"Hz", 1e0f);
	m_upVertScale->SetOrientation(true);
	m_upVertScale->Notify(false);

	::MoveWindow(hwnd, 100, 100, 500, 400, true);
}

void SignalDesigner::Stop()
{
	m_graphics.ShutDown();
	DestroyWindow();
}

void SignalDesigner::doPaint() const
{
	m_upSignalControl->Invalidate(false);
	m_upHorzScale->Invalidate(false);
	m_upVertScale->Invalidate(false);
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
	m_upHorzScale->SetOrthoOffset(fPixBottomOffset);
	m_upVertScale->SetOrthoOffset(fPixLeftOffset);

	m_upSignalControl->Move
	(
		LEFT_OFFSET,
		TOP_OFFSET,
		width  - LEFT_OFFSET - RIGHT_OFFSET,
		height - TOP_OFFSET  - BOTTOM_OFFSET,
		true
	);
	
	m_upHorzScale->Move
	(
		LEFT_OFFSET,
		height - BOTTOM_OFFSET,
		width  - LEFT_OFFSET - RIGHT_OFFSET,
		BOTTOM_OFFSET,
		true
	);

	m_upVertScale->Move
	(
		0_PIXEL,
		TOP_OFFSET,
		LEFT_OFFSET,
		height - TOP_OFFSET  - BOTTOM_OFFSET,
		true
	);

	Trigger();  // cause repaint
	return true;
}
