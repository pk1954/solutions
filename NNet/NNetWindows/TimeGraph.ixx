// TimeGraph.ixx
//
// NNetWindows

module;

#include <cassert>
#include <math.h>
#include <Windows.h>

export module NNetWin32:TimeGraph;

import Types;
import PixFpDimension;
import Direct2D;
import GraphicsWindow;
import NNetModel;

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
	SigGenId                 GetSigGenIdSelected() const { return m_pNMWI->GetSigGenIdSelected(); }
	SigGenStaticData const * GetSigGenStaticData() const { return & GetSigGenSelected()->GetStaticData(); }
	NNetParameters         * GetParams          () const { return & m_pNMWI->GetParams(); }

	fPixel PaintCurve
	(
		auto               getPoint,
		fMicroSecs   const timeStart0,
		fMicroSecs   const timeEnd,
		fPixel       const fPixWidth,
		D2D1::ColorF const color          
	) const
	{
		fMicroSecs const usResolution  { GetParams()->TimeResolution() };
		fMicroSecs const timeStart     { usResolution * Cast2Float(floor(timeStart0 / usResolution)) };
		fPixel           fPixMinSignal { fPixel::MAX_VAL() };
		fPixelPoint      prevPoint     { getPoint(timeStart) };
		if (prevPoint.IsNull())
			return fPixMinSignal;

		ID2D1SolidColorBrush * pBrush { m_upGraphics->CreateBrush(color) };

		if (m_pHorzCoord->Transform2fPixelSize(usResolution) >= 3._fPixel)
		{
			for (fMicroSecs time = timeStart + usResolution; time < timeEnd; time += usResolution)
			{
				fPixelPoint const actPoint { getPoint(time) };
				if (actPoint.IsNull())
					break;
				if (actPoint.GetY() < fPixMinSignal)
					fPixMinSignal = actPoint.GetY();

				fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
				m_upGraphics->DrawLine(prevPoint, stepPoint, fPixWidth, *pBrush);
				m_upGraphics->DrawLine(stepPoint, actPoint,  fPixWidth, *pBrush);
				prevPoint = actPoint;
			}
		}
		else
		{
			fPixel fPixYmin { prevPoint.GetY() };

			for (fMicroSecs time = timeStart + usResolution; time < timeEnd; time += usResolution)
			{
				fPixelPoint actPoint { getPoint(time) };
				assert(actPoint.IsNotNull());
				if (actPoint.GetY() < fPixYmin)
				{
					fPixYmin = actPoint.GetY();
					if (fPixYmin < fPixMinSignal)
						fPixMinSignal = fPixYmin;
				}
				if (actPoint.GetX() - prevPoint.GetX() > 1.0_fPixel)
				{
					actPoint.SetY(fPixYmin);
					m_upGraphics->DrawLine(prevPoint, actPoint, fPixWidth, *pBrush);
					prevPoint = actPoint;
					fPixYmin = fPixel::MAX_VAL();
				}
			}
		}
		SafeRelease(& pBrush);
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