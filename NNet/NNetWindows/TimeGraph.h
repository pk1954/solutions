// #include "Neuron.h"
//
// NNetWindows

#pragma once

#include "Direct2D.h"
#include "MoreTypes.h"
#include "PixFpDimension.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"
#include "NNetModelWriterInterface.h"
#include "win32_graphicsWindow.h"

class TimeGraph : public GraphicsWindow
{
public:
	TimeGraph
	(
		HWND                   const hwndParent,
		PixFpDimension<fMicroSecs> * pHorzCoord
	);

	~TimeGraph();

	void SetModelInterface(NNetModelWriterInterface * const);

	void SetRightBorder(fPixel const b) { m_fPixRightBorder = b; }

protected:

	fPixel const STD_WIDTH  { 1.0_fPixel };
	fPixel const HIGH_WIDTH { 3.0_fPixel };

	fPixel m_fPixRightBorder { 0.0_fPixel };
	fPixel m_fPixRight       { 0.0_fPixel };
	fPixel m_fPixBottom      { 0.0_fPixel };
	fPixel m_fPixLeft        { 0.0_fPixel };

	PixFpDimension<fMicroSecs> * m_pHorzCoord { nullptr };
	NNetModelWriterInterface   * m_pNMWI      { nullptr };

	SignalGenerator  const * GetSigGenSelected  () const { return m_pNMWI->GetSigGenSelected(); }
	SignalGenerator        * GetSigGenSelected  ()       { return m_pNMWI->GetSigGenSelected(); }
	SigGenStaticData const * GetSigGenStaticData() const { return & GetSigGenSelected()->GetStaticData(); }
	Param                  * GetParams          () const { return & m_pNMWI->GetParams(); }

	fPixel PaintCurve
	(
		auto               getPoint,
		fMicroSecs   const timeStart0,
		fMicroSecs   const timeEnd,
		fPixel       const fPixWidth,
		D2D1::ColorF const color          
	) const
	{
		fMicroSecs const usPixelSize  { m_pHorzCoord->GetPixelSize() };
		fMicroSecs const usResolution { GetParams()->TimeResolution() };
		fMicroSecs const usIncrement  { max(usPixelSize, usResolution) };
		fMicroSecs const timeStart    { usIncrement * floor(timeStart0 / usIncrement) };

		fPixel      fPixMinSignal { fPixel::MAX_VAL() };
		fPixelPoint prevPoint     { getPoint(timeStart) };

		for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
		{
			fPixelPoint const actPoint   { getPoint(time) };
			fPixel      const fPixSignal { actPoint.GetY() };
			if (fPixSignal < fPixMinSignal)
				fPixMinSignal = fPixSignal;
			if (actPoint.GetX() - prevPoint.GetX() >= 2._fPixel)
			{
				fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
				m_upGraphics->DrawLine(prevPoint, stepPoint, fPixWidth, color);
				m_upGraphics->DrawLine(stepPoint, actPoint,  fPixWidth, color);
			}
			else
			{
				m_upGraphics->DrawLine(prevPoint, actPoint, fPixWidth, color);
			}
			prevPoint = actPoint;
		}
		return fPixMinSignal;
	}

	bool OnSize(PIXEL const, PIXEL const) override;

	fMicroSecs getTime(fPixelPoint const &) const;
	fPixel     xTime  (fMicroSecs  const  ) const;

	fPixel xLeft  () const { return m_fPixLeft;   }
	fPixel xRight () const { return m_fPixRight - m_fPixRightBorder; }
	fPixel yBottom() const { return m_fPixBottom; }
};