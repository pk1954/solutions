// TimeGraph.ixx
//
// NNetWindows

module;

#include "NNetParameters.h"
#include "SignalGenerator.h"
#include "NNetModelWriterInterface.h"

export module TimeGraph;

import MoreTypes;
import PixFpDimension;
import Direct2D;
import GraphicsWindow;

export class TimeGraph : public GraphicsWindow
{
public:
	TimeGraph
	(
		HWND                   const hwndParent,
		PixFpDimension<fMicroSecs> * pHorzCoord
	);

	~TimeGraph() override;

	virtual void SetModelInterface(NNetModelWriterInterface * const);

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
		fMicroSecs const usPixelSize   { m_pHorzCoord->GetPixelSize() };
		fMicroSecs const usResolution  { GetParams()->TimeResolution() };
		fMicroSecs const usIncrement   { max(usPixelSize, usResolution) };
		fMicroSecs const timeStart     { usIncrement * floor(timeStart0 / usIncrement) };
		fPixel           fPixMinSignal { fPixel::MAX_VAL() };
		fPixelPoint      prevPoint     { getPoint(timeStart) };
		if (prevPoint.IsNotNull())
		{
			ID2D1SolidColorBrush * pBrush { m_upGraphics->CreateBrush(color) };
			for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
			{
				fPixelPoint const actPoint { getPoint(time) };
				if (actPoint.IsNull())
					break;
				if (actPoint.GetY() < fPixMinSignal)
					fPixMinSignal = actPoint.GetY();
				if (actPoint.GetX() - prevPoint.GetX() >= 2._fPixel)
				{
					fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
					m_upGraphics->DrawLine(prevPoint, stepPoint, fPixWidth, pBrush);
					m_upGraphics->DrawLine(stepPoint, actPoint,  fPixWidth, pBrush);
				}
				else
				{
					m_upGraphics->DrawLine(prevPoint, actPoint, fPixWidth, pBrush);
				}
				prevPoint = actPoint;
			}
			SafeRelease(& pBrush);
		}
		return fPixMinSignal;
	}

	bool OnSize(PIXEL const, PIXEL const) override;

	fMicroSecs getTime(fPixelPoint const &) const;
	fMicroSecs getTime(fPixel      const  ) const;
	fPixel     xTime  (fMicroSecs  const  ) const;

	fPixel xLeft  () const { return m_fPixLeft;   }
	fPixel xRight () const { return m_fPixRight - m_fPixRightBorder; }
	fPixel yBottom() const { return m_fPixBottom; }
};