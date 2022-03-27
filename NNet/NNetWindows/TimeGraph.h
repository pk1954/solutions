// TimeGraph.h
//
// NNetWindows

#pragma once

#include "Direct2D.h"
#include "MoreTypes.h"
#include "PixFpDimension.h"
#include "SignalGenerator.h"
#include "win32_graphicsWindow.h"

class TimeGraph : public GraphicsWindow
{
public:
	TimeGraph
	(
		HWND                   const hwndParent,
		SignalGenerator      * const pSigGen,
		PixFpDimension<fMicroSecs> * pHorzCoord
	)
	  : m_pHorzCoord(pHorzCoord),
		m_pSigGen(pSigGen)
	{
		GraphicsWindow::Initialize
		(
			hwndParent, 
			L"ClassTimeGraph", 
			WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
		);
		m_pSigGen   ->RegisterObserver(*this); // signal generator data can be changed from outside
		m_pHorzCoord->RegisterObserver(*this); 
	};

	~TimeGraph()
	{
		m_pHorzCoord->UnregisterObserver(*this); 
		m_pSigGen   ->UnregisterObserver(*this);
	}

protected:
	fPixel const STD_WIDTH  { 1.0_fPixel };
	fPixel const HIGH_WIDTH { 3.0_fPixel };

	fPixel m_fPixRight  { 0.0_fPixel };
	fPixel m_fPixBottom { 0.0_fPixel };
	fPixel m_fPixLeft   { 0.0_fPixel };

	PixFpDimension<fMicroSecs> * m_pHorzCoord { nullptr };
	SignalGenerator            * m_pSigGen;

	fMicroSecs getTime(fPixelPoint const & p) const 
	{ 
		return m_pHorzCoord->Transform2logUnitPos(p.GetX()); 
	}

	void paintCurve
	(
		auto               getPoint,
		D2D1::ColorF const col          
	) const
	{
		fMicroSecs const usResolution { m_pSigGen->GetParams().GetParameterValue(ParamType::Value::timeResolution) };
		fMicroSecs const usPixelSize  { m_pHorzCoord->GetPixelSize() };
		fMicroSecs const usIncrement  { max(usPixelSize, usResolution) };
		fMicroSecs const timeStart    { 0.0_MicroSecs };
		fMicroSecs const usMax        { getTime(m_fPixRight) };
		fMicroSecs const timeEnd      { min(usMax, m_pSigGen->CutoffTime()) };
		fPixelPoint      prevPoint    { getPoint(timeStart) };

		for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
		{
			fPixelPoint const actPoint  { getPoint(time) };
			fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
			m_upGraphics->DrawLine(prevPoint, stepPoint, STD_WIDTH, col);
			m_upGraphics->DrawLine(stepPoint, actPoint,  STD_WIDTH, col);
			prevPoint = actPoint;
		}
	}

	bool OnSize(PIXEL const width, PIXEL const height) override
	{
		GraphicsWindow::OnSize(width, height);
		m_fPixRight  = Convert2fPixel(width);
		m_fPixBottom = Convert2fPixel(height);
		return true;
	}

	fPixel xTime(fMicroSecs const time) const 
	{ 
		return fPixel(m_pHorzCoord->Transform2fPixelPos(time)); 
	}

	fPixel xLeft  () const { return m_fPixLeft;   }
	fPixel xRight () const { return m_fPixRight;  }
	fPixel yBottom() const { return m_fPixBottom; }

private:


};