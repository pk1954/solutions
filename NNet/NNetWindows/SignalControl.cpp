// SignalControl.cpp
//
// NNetWindows

#include "stdafx.h"
#include <algorithm>  // min/max/abs templates
#include "Resource.h"
#include "PointType.h"
#include "NNetParameters.h"
#include "InputConnector.h"
#include "ComputeThread.h"
#include "SignalControl.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"

using std::max;

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	ComputeThread        const & computeThread,
	NNetModelCommands          & commands,
	Observable                 & runObservable,
	SignalGenerator            & sigGen,
	PixFpDimension<fMicroSecs> * pHorzCoord
)
  : TimeGraph(hwndParent, sigGen, pHorzCoord),
	m_computeThread(computeThread),
	m_commands(commands),
	m_runObservable(runObservable),
	m_sigGen(sigGen)
{
	m_runObservable     .RegisterObserver(*this);
	m_sigGen.GetParams().RegisterObserver(*this);
}

SignalControl::~SignalControl()
{
	m_sigGen.GetParams().UnregisterObserver(*this);
	m_runObservable     .UnregisterObserver(*this);
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
		auto const pntVolt       { pixPntVolt(time) };
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
	fMicroSecs const usPixelSize  { m_pHorzCoord->GetPixelSize() };
	fMicroSecs const usResolution { m_sigGen.GetParamsC().TimeResolution() };
	fMicroSecs const usIncrement  { max(usPixelSize, usResolution) };
	if (m_pVertCoordFreq)
		PaintCurve([this](fMicroSecs const t){ return pixPntFreq(t); }, usIncrement, getColor(tColor::FREQ));
	if (m_pVertCoordVolt)
		PaintCurve([this](fMicroSecs const t){ return pixPntVolt(t); }, usIncrement, getColor(tColor::VOLT));
}

void SignalControl::ScaleTimeCoord()
{
	fMicroSecs const umMaxVisible { getTime(m_fPixRight) };
	fMicroSecs const umCutoff     { m_sigGen.GetData().CutoffTime() };
	float      const factor       { umCutoff / umMaxVisible };
	*m_pHorzCoord *= factor;
}

void SignalControl::ScaleFreqCoord()
{
	fHertz const fHertzMaxVisible { getFreq(0.0_fPixel) };
	fHertz const fHertzPeak       { m_sigGen.GetData().GetFreq().Peak() };
	float  const factor           { fHertzPeak / (fHertzMaxVisible * 0.9f) };
	*m_pVertCoordFreq *= factor;
}

void SignalControl::ScaleVoltCoord()
{
	mV    const mVmaxVisible { getVolt(0.0_fPixel) };
	mV    const mVpeak       { m_sigGen.GetData().GetAmplit().Peak() };
	float const factor       { mVpeak / (mVmaxVisible * 0.9f) };
	*m_pVertCoordVolt *= factor;
}

bool SignalControl::OnSize(PIXEL const width, PIXEL const height)
{
	TimeGraph::OnSize(width, height);
	//if (m_fPixRight > 0.0_fPixel)
	//	ScaleTimeCoord();
	if (m_pVertCoordFreq)
		ScaleFreqCoord();
	if (m_pVertCoordVolt)
		ScaleVoltCoord();
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
	SigGenData        m_sigGenData(m_sigGen.GetData());

	if (wParam & MK_LBUTTON)
	{
		switch (m_moveMode)
		{
		case tPos::TIME:
			m_sigGenData.SetPeakTime(getTime(fPixCrsrPos));
			break;
		case tPos::BASE_FREQ:
			m_sigGenData.SetFreqBase(getFreq(fPixCrsrPos));
			break;
		case tPos::PEAK_FREQ:
			m_sigGenData.SetFreqPeak(getFreq(fPixCrsrPos));
			break;
		case tPos::TIME_FREQ:
			m_sigGenData.SetFreqPeak(getFreq(fPixCrsrPos));
			m_sigGenData.SetPeakTime(getTime(fPixCrsrPos));
			break;
		case tPos::BASE_VOLT:
			m_sigGenData.SetAmplBase(getVolt(fPixCrsrPos));
			break;
		case tPos::PEAK_VOLT:
			m_sigGenData.SetAmplPeak(getVolt(fPixCrsrPos));
			break;
		case tPos::TIME_VOLT:
			m_sigGenData.SetAmplPeak(getVolt(fPixCrsrPos));
			m_sigGenData.SetPeakTime(getTime(fPixCrsrPos));
			break;
		default:
			break;
		}
		m_commands.SetSigGenData(m_sigGen, m_sigGenData);
		Notify(true);
	}
	else  // left button not pressed: select
	{
		setPos(fPixCrsrPos);
		Trigger();   // cause repaint
	}
	TimeGraph::OnMouseMove(wParam, lParam);
}

void SignalControl::OnMouseLeave()
{
	m_moveMode = tPos::NONE;
	Trigger();   // cause repaint
	GraphicsWindow::OnMouseLeave();
}
