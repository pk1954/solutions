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

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	ComputeThread        const & computeThread,
	NNetModelCommands          & commands,
	SignalGenerator            & sigGen,
	PixFpDimension<fMicroSecs> * pHorzCoord
)
  :	m_pHorzCoord(pHorzCoord),
	m_computeThread(computeThread),
	m_sigGen(sigGen),
	m_commands(commands)
{
	m_sigGen.RegisterObserver(*this); // signal generator data can be changed fron outside
	GraphicsWindow::Initialize(hwndParent, L"ClassSignalControl", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
}

SignalControl::~SignalControl()
{
	m_sigGen.UnregisterObserver(*this);
	m_pHorzCoord->UnregisterObserver(*this); 
	m_pVertCoordFreq->UnregisterObserver(*this);
	m_pVertCoordVolt->UnregisterObserver(*this);
}

void SignalControl::SetVertCoordFreq(PixFpDimension<fHertz> * pCoord)
{
	m_pVertCoordFreq = pCoord;
}

void SignalControl::SetVertCoordVolt(PixFpDimension<mV>* pCoord)
{
	m_pVertCoordVolt = pCoord;
}

void SignalControl::drawLine
(
	tColor const colType,
	tPos   const posStart,
	tPos   const posEnd
) const
{
	fPixelPoint const fPixPntStart { getPos(posStart) };
	fPixelPoint const fPixPntEnd   { getPos(posEnd) };
	if (fPixPntStart.IsNotNull() && fPixPntEnd.IsNotNull())
	{
		fPixel       width { (colType == tColor::HIGH) ? HIGH_WIDTH : STD_WIDTH };
		D2D1::ColorF col   { getColor(colType) };
		col.a = 0.2f;
		m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, col);
	}
}

void SignalControl::drawDiam
(
	tColor const colType,
	tPos   const pos
) const
{
	fPixelPoint const fPixPnt { getPos(pos) };
	if (fPixPnt.IsNotNull())
	{
		fPixel       const size { (colType == tColor::HIGH) ? HIGH_DIAMOND : STD_DIAMOND };
		D2D1::ColorF const col  { getColor(colType) };
		m_upGraphics->FillDiamond(fPixPnt, size, col);
	}
}

void SignalControl::paintRunControls() const
{
	auto time { m_sigGen.TimeTilTrigger() };

	if (m_pVertCoordFreq)
	{
		auto pntFreq       { pixPntFreq(time) };
		auto pntFreqLeft   { fPixelPoint(       xLeft(), pntFreq.GetY()) };
		auto pntFreqBottom { fPixelPoint(pntFreq.GetX(),      yBottom()) };
		m_upGraphics->DrawLine(pntFreq, pntFreqLeft  , STD_WIDTH, getColor(tColor::FREQ));
		m_upGraphics->DrawLine(pntFreq, pntFreqBottom, STD_WIDTH, getColor(tColor::TIME));
		m_upGraphics->FillDiamond(pntFreq, STD_DIAMOND, getColor(tColor::FREQ));
	}

	if (m_pVertCoordVolt)
	{
		auto pntVolt       { pixPntVolt(time) };
		auto pntVoltRight  { fPixelPoint(      xRight(), pntVolt.GetY()) };
		auto pntVoltBottom { fPixelPoint(pntVolt.GetX(),      yBottom()) };
		m_upGraphics->DrawLine(pntVolt, pntVoltRight , STD_WIDTH, getColor(tColor::VOLT));
		m_upGraphics->DrawLine(pntVolt, pntVoltBottom, STD_WIDTH, getColor(tColor::TIME));
		m_upGraphics->FillDiamond(pntVolt, STD_DIAMOND, getColor(tColor::VOLT));
	}
}

void SignalControl::calcHandles()
{
	m_handles[static_cast<int>(tPos::NONE)] = fPixelPoint();
	m_handles[static_cast<int>(tPos::TIME)] = fPixelPoint(xPeak(), yBottom());

	if (m_pVertCoordFreq)
	{
		m_handles[static_cast<int>(tPos::BASE_FREQ)] = fPixelPoint(xLeft (), yBaseFreq());
		m_handles[static_cast<int>(tPos::PEAK_FREQ)] = fPixelPoint(xLeft (), yPeakFreq());
		m_handles[static_cast<int>(tPos::TIME_FREQ)] = fPixelPoint(xPeak (), yPeakFreq());
		m_handles[static_cast<int>(tPos::BASA_FREQ)] = fPixelPoint(xRight(), yBaseFreq());
	}
	if (m_pVertCoordVolt)
	{
		fPixel const dirPos  { m_pVertCoordFreq ? xRight() : xLeft () };
		fPixel const dirPosA { m_pVertCoordFreq ? xLeft () : xRight() };
		m_handles[static_cast<int>(tPos::BASE_VOLT)] = fPixelPoint(dirPos,  yBaseVolt());
		m_handles[static_cast<int>(tPos::PEAK_VOLT)] = fPixelPoint(dirPos,  yPeakVolt());
		m_handles[static_cast<int>(tPos::TIME_VOLT)] = fPixelPoint(xPeak(), yPeakVolt());
		m_handles[static_cast<int>(tPos::BASA_VOLT)] = fPixelPoint(dirPosA, yBaseVolt());
	}
}

void SignalControl::highlightMovedObject() const
{
	switch (m_moveMode)
	{
	case tPos::TIME:
		drawLine(tColor::HIGH, tPos::TIME, tPos::TIME_VOLT);
		drawLine(tColor::HIGH, tPos::TIME, tPos::TIME_FREQ);
		drawDiam(tColor::HIGH, tPos::TIME);
		break;
	case tPos::PEAK_FREQ:
		drawLine(tColor::HIGH, tPos::PEAK_FREQ, tPos::TIME_FREQ);
		drawDiam(tColor::HIGH, tPos::PEAK_FREQ);
		break;
	case tPos::BASE_FREQ:
		drawLine(tColor::HIGH, tPos::BASE_FREQ, tPos::BASA_FREQ);
		drawDiam(tColor::HIGH, tPos::BASE_FREQ);
		break;
	case tPos::TIME_FREQ:
		drawLine(tColor::HIGH, tPos::TIME_FREQ, tPos::PEAK_FREQ);
		drawLine(tColor::HIGH, tPos::TIME,      tPos::TIME_VOLT);
		drawLine(tColor::HIGH, tPos::TIME,      tPos::TIME_FREQ);
		drawDiam(tColor::HIGH, tPos::TIME_FREQ);
		break;
	case tPos::BASE_VOLT:
		drawLine(tColor::HIGH, tPos::BASE_VOLT, tPos::BASA_VOLT);
		drawDiam(tColor::HIGH, tPos::BASE_VOLT);
		break;
	case tPos::PEAK_VOLT:
		drawLine(tColor::HIGH, tPos::TIME_VOLT, tPos::PEAK_VOLT);
		drawDiam(tColor::HIGH, tPos::PEAK_VOLT);
		break;
	case tPos::TIME_VOLT:
		drawLine(tColor::HIGH, tPos::TIME_VOLT, tPos::PEAK_VOLT);
		drawLine(tColor::HIGH, tPos::TIME,      tPos::TIME_VOLT);
		drawLine(tColor::HIGH, tPos::TIME,      tPos::TIME_FREQ);
		drawDiam(tColor::HIGH, tPos::TIME_VOLT);
		break;
	default:
		break;
	}
}

void SignalControl::paintEditControls() const
{
	if (m_moveMode != tPos::NONE)
		highlightMovedObject();
	if (m_pVertCoordFreq)
	{
		drawLine(tColor::FREQ, tPos::BASE_FREQ, tPos::BASA_FREQ);
		drawLine(tColor::FREQ, tPos::PEAK_FREQ, tPos::TIME_FREQ);
		drawLine(tColor::TIME, tPos::TIME     , tPos::TIME_FREQ);
		drawDiam(tColor::FREQ, tPos::BASE_FREQ);
		drawDiam(tColor::FREQ, tPos::PEAK_FREQ);
		drawDiam(tColor::FREQ, tPos::TIME_FREQ);
	}
	if (m_pVertCoordVolt)
	{
		drawLine(tColor::VOLT, tPos::BASE_VOLT, tPos::BASA_VOLT);
		drawLine(tColor::VOLT, tPos::TIME_VOLT, tPos::PEAK_VOLT);
		drawLine(tColor::TIME, tPos::TIME     , tPos::TIME_VOLT);
		drawDiam(tColor::VOLT, tPos::BASE_VOLT);
		drawDiam(tColor::VOLT, tPos::PEAK_VOLT);
		drawDiam(tColor::VOLT, tPos::TIME_VOLT);
	}
	if (m_pHorzCoord)
	{
		drawDiam(tColor::TIME, tPos::TIME);
	}
}

void SignalControl::DoPaint()
{
	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);
	if ( !m_computeThread.IsRunning() )
	{
		calcHandles();
		paintEditControls();
	}
	else if (m_sigGen.IsTriggerActive())
	{
		paintRunControls();
	}
	if (m_pVertCoordFreq)
		paintCurve([this](fMicroSecs const t){ return pixPntFreq(t); }, tColor::FREQ);
	if (m_pVertCoordVolt)
		paintCurve([this](fMicroSecs const t){ return pixPntVolt(t); }, tColor::VOLT);
}

bool SignalControl::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixRight  = Convert2fPixel(width);
	m_fPixBottom = Convert2fPixel(height);
	Trigger();  // cause repaint
	return true;
}

void SignalControl::setPos(fPixelPoint const & pos)
{
	m_moveMode = tPos::NONE;
	fPixel fPixDistBest { 10000._fPixel };
	
	if (m_pHorzCoord)
	{
		testPos(tPos::TIME,      pos, fPixDistBest);
	}
	
	if (m_pVertCoordFreq)
	{
		testPos(tPos::BASE_FREQ, pos, fPixDistBest);
		testPos(tPos::PEAK_FREQ, pos, fPixDistBest);
		testPos(tPos::TIME_FREQ, pos, fPixDistBest);
	}

	if (m_pVertCoordVolt)
	{
		testPos(tPos::BASE_VOLT, pos, fPixDistBest);
		testPos(tPos::PEAK_VOLT, pos, fPixDistBest);
		testPos(tPos::TIME_VOLT, pos, fPixDistBest);
	}
}

void SignalControl::testPos
(
	tPos        const tPosTest, 
	fPixelPoint const posCrsr, 
	fPixel          & fPixDistBest
)
{
	fPixel const fPixCandidate = Distance(posCrsr, getPos(tPosTest));
	if ((fPixCandidate <= 10.0_fPixel) && (fPixCandidate < fPixDistBest))
	{
		fPixDistBest = fPixCandidate;
		m_moveMode   = tPosTest;
	}
}

void SignalControl::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };

	if (wParam & MK_LBUTTON)
	{
		m_sigGenNew.SetParams(m_sigGen);
		switch (m_moveMode)
		{
		case tPos::TIME:
			m_sigGenNew.SetTimePeak(getTime(fPixCrsrPos));
			break;
		case tPos::BASE_FREQ:
			m_sigGenNew.SetBaseFreq(getFreq(fPixCrsrPos));
			break;
		case tPos::PEAK_FREQ:
			m_sigGenNew.SetPeakFreq(getFreq(fPixCrsrPos));
			break;
		case tPos::TIME_FREQ:
			m_sigGenNew.SetPeakFreq(getFreq(fPixCrsrPos));
			m_sigGenNew.SetTimePeak(getTime(fPixCrsrPos));
			break;
		case tPos::BASE_VOLT:
			m_sigGenNew.SetBaseVolt(getVolt(fPixCrsrPos));
			break;
		case tPos::PEAK_VOLT:
			m_sigGenNew.SetPeakVolt(getVolt(fPixCrsrPos));
			break;
		case tPos::TIME_VOLT:
			m_sigGenNew.SetPeakVolt(getVolt(fPixCrsrPos));
			m_sigGenNew.SetTimePeak(getTime(fPixCrsrPos));
			break;
		default:
			break;
		}
		m_commands.SetStimulusParams(m_sigGen, m_sigGenNew);
	}
	else  // left button not pressed: select
	{
		setPos(fPixCrsrPos);
		Trigger();   // cause repaint
	}
	TrackMouse();
}

bool SignalControl::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	m_moveMode = tPos::NONE;
	Trigger();   // cause repaint
	return false;
}
