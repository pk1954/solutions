// SignalControl.cpp
//
// NNetSignals

module;

#include <chrono>
#include <algorithm>  // min/max/abs templates
#include <Windows.h>
#include "Resource.h"

module NNetSignals:SignalControl;

import Win32_Util_Resource;
import Types;
import NNetModel;
import NNetCommands;
import :SimuRunning;

using std::max;

using namespace std::chrono;

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	Observable                 & runObservable,
	Observable                 & dynamicModelObservable,
	SimuRunning          const & simuRunning,
	PixFpDimension<fMicroSecs> * pHorzCoord
)
  : NNetTimeGraph(hwndParent),
	m_runObservable(runObservable),
	m_dynamicModelObservable(dynamicModelObservable),
	m_simuRunning(simuRunning)
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
	if (m_pVertScaleFreq)
		vertCoordFreq().UnregisterObserver(*this);
	if (m_pVertScaleVolt)
		vertCoordVolt().UnregisterObserver(*this);
}

void SignalControl::Snap2Grid(bool const b) 
{ 
	m_bSnap2Grid = b; 
	Trigger();   // cause repaint
}

void SignalControl::SetGridDimFactor(float const f)
{
	m_fGridDimFactor = f;
	Trigger();
}

void SignalControl::SetVertScaleFreq(Scale<fHertz> * pScale)
{
	if (m_pVertScaleFreq)
		vertCoordFreq().UnregisterObserver(*this);
	m_pVertScaleFreq = pScale;
	if (m_pVertScaleFreq)
		vertCoordFreq().RegisterObserver(*this);
}

void SignalControl::SetVertScaleVolt(Scale<mV> * pScale)
{
	if (m_pVertScaleVolt)
		vertCoordVolt().UnregisterObserver(*this);
	m_pVertScaleVolt = pScale;
	if (m_pVertScaleVolt)
		vertCoordVolt().RegisterObserver(*this);
}

void SignalControl::SetHorzScale(Scale<fMicroSecs>* pScale)
{
	if (m_pHorzScale)
		m_pHorzScale->GetDimension().UnregisterObserver(*this);
	m_pHorzScale = pScale;
	if (m_pHorzScale)
	{
		SetHorzCoord(&m_pHorzScale->GetDimension());
		m_pHorzScale->GetDimension().RegisterObserver(*this);
	}
}

fHertz SignalControl::getFreq(fPixel const fPixY) const
{
	fHertz fRes { vertCoordFreqC().Transform2logUnitPos(getY(fPixY)) };
	if (m_bSnap2Grid)
	{
		fHertz const fRaster { m_pVertScaleFreq->GetRaster() };
		fRes = fRaster * round(fRes / fRaster);
	}
	return fRes;
}

mV SignalControl::getVolt(fPixel const fPixY) const
{
	mV fRes { vertCoordVoltC().Transform2logUnitPos(getY(fPixY)) };
	if (m_bSnap2Grid)
	{
		mV const fRaster { m_pVertScaleVolt->GetRaster() };
		fRes = fRaster * round(fRes / fRaster);
	}
	return fRes;
}

fMicroSecs SignalControl::getTime(fPixelPoint const &p) const
{
	fMicroSecs fRes { NNetTimeGraph::GetTime(p.GetX()) };
	if (m_bSnap2Grid)
	{
		fMicroSecs const fRaster { m_pHorzScale->GetRaster() };
		fRes = fRaster * round(fRes / fRaster);
	}
	return fRes;
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
		fPixel width { (colType == tColor::HIGH) ? HIGH_WIDTH : STD_WIDTH };
		Color  col   { getColor(colType) };
		col.a = 0.2f;

		if ((fPixPntStart.GetX() != fPixPntEnd.GetX()) && (fPixPntStart.GetY() != fPixPntEnd.GetY()))
		{
			int x = 42;
		}

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
		fPixel const size { (colType == tColor::HIGH) ? HIGH_DIAMOND : STD_DIAMOND };
		Color  const col  { getColor(colType) };
		m_upGraphics->FillDiamond(fPixPnt, size, col);
	}
}

void SignalControl::paintRunControls(fMicroSecs const time) const
{
	if (m_pVertScaleFreq)
	{
		fPixelPoint pntFreq       { pixPntStimulusFreq(time) };
		fPixelPoint pntFreqLeft   { fPixelPoint(       xLeft(), pntFreq.GetY()) };
		fPixelPoint pntFreqBottom { fPixelPoint(pntFreq.GetX(), yBottom())      };
		m_upGraphics->DrawLine(pntFreq, pntFreqLeft,   1.0_fPixel, getColor(tColor::FREQ));
		m_upGraphics->DrawLine(pntFreq, pntFreqBottom, 1.0_fPixel, getColor(tColor::TIME));
		m_upGraphics->FillDiamond(pntFreq, STD_DIAMOND, getColor(tColor::FREQ));
	}

	if (m_pVertScaleVolt)
	{
		fPixelPoint const pntVolt       { pixPntStimulusVolt(time) };
		fPixel      const dirPos        { m_pVertScaleFreq ? xRight() : xLeft () };
		fPixelPoint const pntVoltBase   { fPixelPoint(        dirPos, pntVolt.GetY()) };
		fPixelPoint const pntVoltBottom { fPixelPoint(pntVolt.GetX(), yBottom())      };
		m_upGraphics->DrawLine(pntVolt, pntVoltBase,   1.0_fPixel, getColor(tColor::VOLT));
		m_upGraphics->DrawLine(pntVolt, pntVoltBottom, 1.0_fPixel, getColor(tColor::TIME));
		m_upGraphics->FillDiamond(pntVolt, STD_DIAMOND, getColor(tColor::VOLT));
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

void SignalControl::calcHandles()
{
	m_handles[static_cast<int>(tPos::NONE)] = fPixelPoint();
	m_handles[static_cast<int>(tPos::TIME)] = fPixelPoint(xPeak(), yBottom());

	if (m_pVertScaleFreq)
	{
		m_handles[static_cast<int>(tPos::BASE_FREQ)] = fPixelPoint(xLeft(), yBaseFreq());
		m_handles[static_cast<int>(tPos::PEAK_FREQ)] = fPixelPoint(xLeft(), yPeakFreq());
		m_handles[static_cast<int>(tPos::TIME_FREQ)] = fPixelPoint(xPeak(), yPeakFreq());
		m_handles[static_cast<int>(tPos::BASA_FREQ)] = fPixelPoint(xRight(), yBaseFreq());
	}
	if (m_pVertScaleVolt)
	{
		fPixel const dirPos  { m_pVertScaleFreq ? xRight() : xLeft() };
		fPixel const dirPosA { m_pVertScaleFreq ? xLeft() : xRight() };
		m_handles[static_cast<int>(tPos::BASE_VOLT)] = fPixelPoint(dirPos, yBaseAmplit());
		m_handles[static_cast<int>(tPos::PEAK_VOLT)] = fPixelPoint(dirPos, aPeakAmplit());
		m_handles[static_cast<int>(tPos::TIME_VOLT)] = fPixelPoint(xPeak(), aPeakAmplit());
		m_handles[static_cast<int>(tPos::BASA_VOLT)] = fPixelPoint(dirPosA, yBaseAmplit());
	}
}

void SignalControl::paintEditControls() const
{
	if (m_moveMode != tPos::NONE)
		highlightMovedObject();
	if (m_pVertScaleFreq)
	{
		drawLine(tColor::FREQ, tPos::BASE_FREQ, tPos::BASA_FREQ);
		drawLine(tColor::FREQ, tPos::PEAK_FREQ, tPos::TIME_FREQ);
		drawLine(tColor::TIME, tPos::TIME     , tPos::TIME_FREQ);
		drawDiam(tColor::FREQ, tPos::BASE_FREQ);
		drawDiam(tColor::FREQ, tPos::PEAK_FREQ);
		drawDiam(tColor::FREQ, tPos::TIME_FREQ);
	}
	if (m_pVertScaleVolt)
	{
		drawLine(tColor::VOLT, tPos::BASE_VOLT, tPos::BASA_VOLT);
		drawLine(tColor::VOLT, tPos::TIME_VOLT, tPos::PEAK_VOLT);
		drawLine(tColor::TIME, tPos::TIME     , tPos::TIME_VOLT);
		drawDiam(tColor::VOLT, tPos::BASE_VOLT);
		drawDiam(tColor::VOLT, tPos::PEAK_VOLT);
		drawDiam(tColor::VOLT, tPos::TIME_VOLT);
	}
	drawDiam(tColor::TIME, tPos::TIME);
}

void SignalControl::PaintGraphics()
{
	if (m_fGridDimFactor > 0.0f)
	{
		if (m_pVertScaleFreq)
			m_pVertScaleFreq->DrawAuxLines(*m_upGraphics.get(), m_fGridDimFactor);
		if (m_pVertScaleVolt && !m_pVertScaleFreq)
			m_pVertScaleVolt->DrawAuxLines(*m_upGraphics.get(), m_fGridDimFactor);
		if (m_pHorzScale)
			m_pHorzScale->DrawAuxLines(*m_upGraphics.get(), m_fGridDimFactor);
	}

	if (SignalGenerator const * pSigGen { GetSigGenSelected() })
	{
		if (!m_simuRunning.IsRunning())
		{
			calcHandles();
			paintEditControls();
		}
		else if (pSigGen->IsStimulusActive())
		{
			paintRunControls(pSigGen->GetStimulusTime());
		}

		if (m_pVertScaleFreq)
			PaintCurve
			(
				[this](fMicroSecs const t){ return pixPntStimulusFreq(t); }, 
				0.0_MicroSecs,
				GetTime(xRight()),
				m_upGraphics->CreateBrush(getColor(tColor::FREQ)),
				STD_WIDTH
			);
		if (m_pVertScaleVolt)
			PaintCurve
			(
				[this](fMicroSecs const t){ return pixPntStimulusVolt(t); }, 
				0.0_MicroSecs,
				GetTime(xRight()),
				m_upGraphics->CreateBrush(getColor(tColor::VOLT)),
				STD_WIDTH
			);
	}
}

float SignalControl::ScaleFactorTimeCoord() const
{
	fMicroSecs const maxVisible { getMaxTime() };
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
	
	testPos(tPos::TIME, pos, fPixDistBest);
	
	if (m_pVertScaleFreq)
	{
		testPos(tPos::BASE_FREQ, pos, fPixDistBest);
		testPos(tPos::PEAK_FREQ, pos, fPixDistBest);
		testPos(tPos::TIME_FREQ, pos, fPixDistBest);
	}

	if (m_pVertScaleVolt)
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

LPARAM SignalControl::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AppendMenu(hPopupMenu, MF_STRING, IDM_SCALE_GRID, L"Grid on/off");
	NNetTimeGraph::AddContextMenuEntries(hPopupMenu);
	return 0L; // will be forwarded to HandleContextMenuCommand
}

bool SignalControl::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	bool bRes = false;

	switch (int const wmId = LOWORD(wParam))
	{
	case IDM_SCALE_GRID:
		SendCommand2Parent(wParam, lParam);
		break;

	default:
		bRes = BaseWindow::OnCommand(wParam, lParam, pixPoint);
		break;
	}

	return bRes;
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
	NNetTimeGraph::OnMouseMove(wParam, lParam);
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
