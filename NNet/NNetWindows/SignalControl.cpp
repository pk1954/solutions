// SignalControl.cpp
//
// NNetWindows

module;

#include <chrono>
#include <algorithm>  // min/max/abs templates
#include <Windows.h>
#include "Resource.h"

module NNetWin32:SignalControl;

import Types;
import NNetModel;
import NNetCommands;
import :ComputeThread;

using std::max;

using namespace std::chrono;

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	ComputeThread        const & computeThread,
	Observable                 & runObservable,
	Observable                 & dynamicModelObservable,
	PixFpDimension<fMicroSecs> * pHorzCoord
)
  : TimeGraph(hwndParent, pHorzCoord),
	m_computeThread(computeThread),
	m_runObservable(runObservable),
	m_dynamicModelObservable(dynamicModelObservable)
{
	m_runObservable         .RegisterObserver(*this);
	m_dynamicModelObservable.RegisterObserver(*this);
	SetRefreshRate(1000ms);
}

SignalControl::~SignalControl()
{
	if (m_pNMWI && m_pNMWI->IsDefined())
		GetParams()->UnregisterObserver(*this);
	m_runObservable         .UnregisterObserver(*this);
	m_dynamicModelObservable.UnregisterObserver(*this);
	if (m_pVertCoordFreq)
		m_pVertCoordFreq->UnregisterObserver(*this);
	if (m_pVertCoordVolt)
		m_pVertCoordVolt->UnregisterObserver(*this);
}

void SignalControl::SetVertCoordFreq(PixFpDimension<fHertz> * pCoord)
{
	if (m_pVertCoordFreq)
		m_pVertCoordFreq->UnregisterObserver(*this);
	m_pVertCoordFreq = pCoord;
	if (m_pVertCoordFreq)
		m_pVertCoordFreq->RegisterObserver(*this);
}

void SignalControl::SetVertCoordVolt(PixFpDimension<mV>* pCoord)
{
	if (m_pVertCoordVolt)
		m_pVertCoordVolt->UnregisterObserver(*this);
	m_pVertCoordVolt = pCoord;
	if (m_pVertCoordVolt)
		m_pVertCoordVolt->RegisterObserver(*this);
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

	if (
		(fPixPntStart.GetX() != fPixPntEnd.GetX()) &&
		(fPixPntStart.GetY() != fPixPntEnd.GetY())
		)
	{
		int x = 42;
	}
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

void SignalControl::paintRunControls(fMicroSecs const time) const
{
	if (m_pVertCoordFreq)
	{
		auto pntFreq       { pixPntStimulusFreq(time) };
		auto pntFreqLeft   { fPixelPoint(       xLeft(), pntFreq.GetY()) };
		auto pntFreqBottom { fPixelPoint(pntFreq.GetX(),      yBottom()) };
		m_upGraphics->DrawLine(pntFreq, pntFreqLeft  , STD_WIDTH, getColor(tColor::FREQ));
		m_upGraphics->DrawLine(pntFreq, pntFreqBottom, STD_WIDTH, getColor(tColor::TIME));
		m_upGraphics->FillDiamond(pntFreq, STD_DIAMOND, getColor(tColor::FREQ));
	}

	if (m_pVertCoordVolt)
	{
		auto const pntVolt       { pixPntStimulusVolt(time) };
		auto const dirPos        { m_pVertCoordFreq ? xRight() : xLeft () };
		auto const pntVoltBase   { fPixelPoint(        dirPos, pntVolt.GetY()) };
		auto const pntVoltBottom { fPixelPoint(pntVolt.GetX(),      yBottom()) };
		m_upGraphics->DrawLine(pntVolt,   pntVoltBase, STD_WIDTH, getColor(tColor::VOLT));
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
		m_handles[static_cast<int>(tPos::BASE_VOLT)] = fPixelPoint(dirPos,  yBaseAmplit());
		m_handles[static_cast<int>(tPos::PEAK_VOLT)] = fPixelPoint(dirPos,  aPeakAmplit());
		m_handles[static_cast<int>(tPos::TIME_VOLT)] = fPixelPoint(xPeak(), aPeakAmplit());
		m_handles[static_cast<int>(tPos::BASA_VOLT)] = fPixelPoint(dirPosA, yBaseAmplit());
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
		drawLine(tColor::HIGH, tPos::TIME,      tPos::TIME_VOLT);  //only needed in integrated mode
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

void SignalControl::PaintGraphics()
{
	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);
	if (SignalGenerator const * pSigGen { GetSigGenSelected() })
	{
		if (!m_computeThread.IsRunning())
		{
			calcHandles();
			paintEditControls();
		}
		else if (pSigGen->IsStimulusActive())
		{
			paintRunControls(pSigGen->GetStimulusTime());
		}
		if (m_pVertCoordFreq)
			PaintCurve
			(
				[this](fMicroSecs const t){ return pixPntStimulusFreq(t); }, 
				0.0_MicroSecs,
				getTime(xRight()),
				STD_WIDTH,
				getColor(tColor::FREQ)
			);
		if (m_pVertCoordVolt)
			PaintCurve
			(
				[this](fMicroSecs const t){ return pixPntStimulusVolt(t); }, 
				0.0_MicroSecs,
				getTime(xRight()),
				STD_WIDTH,
				getColor(tColor::VOLT)
			);
	}
}

float SignalControl::ScaleFactorTimeCoord() const
{
	fMicroSecs const maxVisible { getTime(m_fPixRight) };
	fMicroSecs const maxValue   { GetSigGenStaticData()->CutoffTime() };
	float      const factor     { maxValue / maxVisible };
	return factor;
}

float SignalControl::ScaleFactorFreqCoord() const
{
	fHertz const maxVisible { getFreq(0.0_fPixel) * 0.9f };
	fHertz const maxValue   { GetSigGenStaticData()->GetFrequency().Peak() };
	float  const factor     { maxValue / maxVisible };
	return factor;
}

float SignalControl::ScaleFactorVoltCoord() const
{
	mV    const maxVisible { getVolt(0.0_fPixel) * 0.9f };
	mV    const maxValue   { GetSigGenStaticData()->GetAmplitude().Peak() };
	float const factor     { maxValue / maxVisible };
	return factor;
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
	if (SignalGenerator const * pSigGen { GetSigGenSelected() })
	{
		PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
		fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };
		SigGenStaticData  sigGenData(pSigGen->GetStaticData());

		if (wParam & MK_LBUTTON)
		{
			switch (m_moveMode)
			{
			case tPos::TIME:
				sigGenData.SetPeakTime(getTime(fPixCrsrPos));
				break;
			case tPos::BASE_FREQ:
				sigGenData.SetFreqBase(getFreq(fPixCrsrPos));
				break;
			case tPos::PEAK_FREQ:
				sigGenData.SetFreqPeak(getFreq(fPixCrsrPos));
				break;
			case tPos::TIME_FREQ:
				sigGenData.SetFreqPeak(getFreq(fPixCrsrPos));
				sigGenData.SetPeakTime(getTime(fPixCrsrPos));
				break;
			case tPos::BASE_VOLT:
				sigGenData.SetAmplBase(getVolt(fPixCrsrPos));
				break;
			case tPos::PEAK_VOLT:
				sigGenData.SetAmplPeak(getVolt(fPixCrsrPos));
				break;
			case tPos::TIME_VOLT:
				sigGenData.SetAmplPeak(getVolt(fPixCrsrPos));
				sigGenData.SetPeakTime(getTime(fPixCrsrPos));
				break;
			default:
				break;
			}
			SetSigGenStaticDataCmd::Push(GetSigGenIdSelected(), sigGenData);
			Notify(true);
		}
		else  // left button not pressed: select
		{
			setPos(fPixCrsrPos);
			Trigger();   // cause repaint
		}
	}
	TimeGraph::OnMouseMove(wParam, lParam);
}

void SignalControl::OnMouseLeave()
{
	m_moveMode = tPos::NONE;
	Trigger();   // cause repaint
	GraphicsWindow::OnMouseLeave();
}

bool SignalControl::OnLButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	SetCapture();
	return false;
}

bool SignalControl::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	ReleaseCapture();
	return false;
}
