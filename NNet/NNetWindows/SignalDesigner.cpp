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
	SignalGenerator     & sigGen,
	Observable          & runObservable,
	NNetModelCommands   & commands
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

	SetWindowText(L"SignalDesigner");

	m_upSignalControl = make_unique<SignalControl>
	(
		hwnd, 
		computeThread, 
		sigGen, 
		m_horzCoord, 
		m_vertCoordHertz,
		commands
	);

	runObservable.RegisterObserver(*m_upSignalControl.get());

	m_upHorzScale = make_unique<Scale<fMicroSecs>>(hwnd, false, m_horzCoord);
	m_upVertScale = make_unique<Scale<fHertz    >>(hwnd, true,  m_vertCoordHertz);

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(100._MicroSecs, 1000000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale->SetOrientation(false);
	m_upHorzScale->Show(true);

	m_vertCoordHertz.SetPixelSize(0.25_fHertz);
	m_vertCoordHertz.SetPixelSizeLimits(0.05_fHertz, 1._fHertz); 
	m_vertCoordHertz.SetZoomFactor(1.3f);

	m_upVertScale->SetOrientation(true);
	m_upVertScale->Show(true);

	::MoveWindow(hwnd, 100, 100, 500, 400, true);
}

void SignalDesigner::Stop()
{
	DestroyWindow();
}

void SignalDesigner::OnClose()
{
	Stop();
}

bool SignalDesigner::OnSize(PIXEL const width, PIXEL const height)
{
	static fPixel const fPixLeftOffset   { Convert2fPixel(LEFT_OFFSET  ) };
	static fPixel const fPixBottomOffset { Convert2fPixel(BOTTOM_OFFSET) };

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
