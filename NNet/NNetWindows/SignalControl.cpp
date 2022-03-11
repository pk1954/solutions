// SignalControl.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "PointType.h"
#include "NNetParameters.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "ComputeThread.h"
#include "SignalControl.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"

SignalControl::~SignalControl()
{
	m_sigGen.UnregisterObserver(*this);
}

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	ComputeThread        const & computeThread,
	NNetModelCommands          & commands,
	SignalGenerator            & sigGen,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<fHertz>     & vertCoordFreq,
	PixFpDimension<mV>         & vertCoordVolt
)
  :	m_horzCoord(horzCoord),
	m_vertCoordFreq(vertCoordFreq),
	m_vertCoordVolt(vertCoordVolt),
	m_computeThread(computeThread),
	m_sigGen(sigGen),
	m_commands(commands)
{
	m_sigGen       .RegisterObserver(*this); // signal generator data can be changed fron outside
	m_horzCoord    .RegisterObserver(*this); 
	m_vertCoordFreq.RegisterObserver(*this);
	m_vertCoordVolt.RegisterObserver(*this);
	GraphicsWindow::Initialize(hwndParent, L"ClassSignalControl", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
}

void SignalControl::highlightMovedObject() const
{
	switch (m_moveMode)
	{
	case tMoveMode::TIME:
		drawLine(tColor::HIGH, handlePeakTime(), handleTimeVolt());
		drawLine(tColor::HIGH, handlePeakTime(), handleTimeFreq());
		drawDiam(tColor::HIGH, handlePeakTime());
		break;
	case tMoveMode::PEAK_FREQ:
		drawLine(tColor::HIGH, handlePeakFreq(), handleTimeFreq());
		drawDiam(tColor::HIGH, handlePeakFreq());
		break;
	case tMoveMode::BASE_FREQ:
		drawLine(tColor::HIGH, handleBaseFreq(), fPixelPoint(xRight(), yBaseFreq()));
		drawDiam(tColor::HIGH, handleBaseFreq());
		break;
	case tMoveMode::TIME_FREQ:
		drawLine(tColor::HIGH, handlePeakFreq(), handleTimeFreq());
		drawLine(tColor::HIGH, handlePeakTime(), handleTimeVolt());
		drawLine(tColor::HIGH, handlePeakTime(), handleTimeFreq());
		drawDiam(tColor::HIGH, pixPntFreq(m_sigGen.TimePeak()));
		break;
	case tMoveMode::BASE_VOLT:
		drawLine(tColor::HIGH, handleBaseVolt(), fPixelPoint(xLeft(), yBaseVolt()));
		drawDiam(tColor::HIGH, handleBaseVolt());
		break;
	case tMoveMode::PEAK_VOLT:
		drawLine(tColor::HIGH, handleTimeVolt(), handlePeakVolt());
		drawDiam(tColor::HIGH, handlePeakVolt());
		break;
	case tMoveMode::TIME_VOLT:
		drawLine(tColor::HIGH, handleTimeVolt(), handlePeakVolt());
		drawLine(tColor::HIGH, handlePeakTime(), handleTimeVolt());
		drawLine(tColor::HIGH, handlePeakTime(), handleTimeFreq());
		drawDiam(tColor::HIGH, pixPntVolt(m_sigGen.TimePeak()));
		break;
	default:
		break;
	}
}

void SignalControl::DoPaint()
{
	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);

	if (m_computeThread.IsRunning())
	{
		if (m_sigGen.IsTriggerActive())
		{
			fMicroSecs  time   { m_sigGen.TimeTilTrigger() };
			fPixelPoint pntFreq{ pixPntFreq(time) };
			fPixelPoint pntVolt{ pixPntVolt(time) };
			drawDiam(tColor::VOLT, pixPntVolt(time));
			drawLine(tColor::FREQ, pntFreq, fPixelPoint(xLeft(),        pntFreq.GetY()));
			drawLine(tColor::TIME, pntFreq, fPixelPoint(pntFreq.GetX(), yBottom()));
			drawLine(tColor::VOLT, pntVolt, fPixelPoint(xRight(),       pntVolt.GetY()));
			drawLine(tColor::TIME, pntVolt, fPixelPoint(pntVolt.GetX(), yBottom()));
			drawDiam(tColor::FREQ, pntFreq);
			drawDiam(tColor::VOLT, pntVolt);
		}
	}
	else 
	{
		if (m_moveMode != tMoveMode::NONE)
			highlightMovedObject();
		drawLine(tColor::FREQ, handleBaseFreq(), fPixelPoint(xRight(), yBaseFreq()));
		drawLine(tColor::VOLT, handleBaseVolt(), fPixelPoint(xLeft (), yBaseVolt()));
		drawLine(tColor::VOLT, handleTimeVolt(), handlePeakVolt());
		drawLine(tColor::FREQ, handlePeakFreq(), handleTimeFreq());
		drawLine(tColor::TIME, handlePeakTime(), handleTimeVolt());
		drawLine(tColor::TIME, handlePeakTime(), handleTimeFreq());
		drawDiam(tColor::VOLT, handleBaseVolt());
		drawDiam(tColor::VOLT, handlePeakVolt());
		drawDiam(tColor::VOLT, pixPntVolt(m_sigGen.TimePeak()));
		drawDiam(tColor::FREQ, handleBaseFreq());
		drawDiam(tColor::FREQ, handlePeakFreq());
		drawDiam(tColor::FREQ, pixPntFreq(m_sigGen.TimePeak()));
		drawDiam(tColor::TIME, handlePeakTime());
	}
	paintCurve([this](fMicroSecs const t){ return pixPntFreq(t); }, m_colorFreq );
	paintCurve([this](fMicroSecs const t){ return pixPntVolt(t); }, m_colorVolt );
}

void SignalControl::drawLine
(
	tColor   const   colType,
	fPixelPoint  const & fPixPntStart, 
	fPixelPoint  const & fPixPntEnd
) const
{
	fPixel       width { (colType == tColor::HIGH) ? 3.0_fPixel : 1.0_fPixel };
	D2D1::ColorF col   { getColor(colType) };
	col.a = 0.2f;
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, col);
}

void SignalControl::drawDiam
(
	tColor  const   colType,
	fPixelPoint const & fPixPntPos
) const
{
	fPixel       size { (colType == tColor::HIGH) ? 8.0_fPixel : 5.0_fPixel };
	D2D1::ColorF col  { getColor(colType) };
	m_upGraphics->FillDiamond(fPixPntPos, size, col);
}

bool SignalControl::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixGraphWidth = Convert2fPixel(width);
	Trigger();  // cause repaint
	return true;
}

void SignalControl::setMoveMode(fPixelPoint const & pos)
{
	tMoveMode moveMode     { tMoveMode::NONE };
	fPixel    fPixDistBest { 10000._fPixel };
	fPixel    fPixCandidate;
	fPixCandidate = Distance(pos, handleBaseFreq());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::BASE_FREQ;
	}
	fPixCandidate = Distance(pos, handlePeakFreq());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::PEAK_FREQ;
	}
	fPixCandidate = Distance(pos, handleTimeFreq());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::TIME_FREQ;
	}
	fPixCandidate = Distance(pos, handleBaseVolt());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::BASE_VOLT;
	}
	fPixCandidate = Distance(pos, handlePeakVolt());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::PEAK_VOLT;
	}
	fPixCandidate = Distance(pos, handleTimeVolt());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::TIME_VOLT;
	}
	fPixCandidate = Distance(pos, handlePeakTime());
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		moveMode = tMoveMode::TIME;
	}
	m_moveMode = moveMode;
}

void SignalControl::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };

	if (wParam & MK_LBUTTON)
	{
		fMicroSecs const timeNew { getTime(fPixCrsrPos.GetX()) };
		fHertz     const freqNew { getFreq(fPixCrsrPos.GetY()) };
		mV         const voltNew { getVolt(fPixCrsrPos.GetY()) };
		m_sigGenNew.SetParams(m_sigGen);
		switch (m_moveMode)
		{
		case tMoveMode::TIME:
			m_sigGenNew.SetTimePeak(timeNew);
			break;
		case tMoveMode::BASE_FREQ:
			m_sigGenNew.SetBaseFreq(freqNew);
			break;
		case tMoveMode::PEAK_FREQ:
			m_sigGenNew.SetPeakFreq(freqNew);
			break;
		case tMoveMode::TIME_FREQ:
			m_sigGenNew.SetPeakFreq(freqNew);
			m_sigGenNew.SetTimePeak(timeNew);
			break;
		case tMoveMode::BASE_VOLT:
			m_sigGenNew.SetBaseVolt(voltNew);
			break;
		case tMoveMode::PEAK_VOLT:
			m_sigGenNew.SetPeakVolt(voltNew);
			break;
		case tMoveMode::TIME_VOLT:
			m_sigGenNew.SetPeakVolt(voltNew);
			m_sigGenNew.SetTimePeak(timeNew);
			break;
		default:
			break;
		}
		m_commands.SetStimulusParams(m_sigGen, m_sigGenNew);
	}
	else  // left button not pressed: select
	{
		setMoveMode(fPixCrsrPos);
		Trigger();   // cause repaint
	}
	TrackMouse();
}

bool SignalControl::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	m_moveMode = tMoveMode::NONE;
	Trigger();   // cause repaint
	return false;
}
