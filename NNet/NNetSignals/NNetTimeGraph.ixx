// NNetTimeGraph.ixx
//
// NNetWindows

module;

#include <d2d1helper.h>
#include <cassert>
#include <compare>
#include <math.h>
#include <Windows.h>

export module NNetSignals:NNetTimeGraph;

import Types;
import Scale;
import PixFpDimension;
import Direct2D;
import TimeGraph;
import NNetModel;
import Signals;

export class NNetTimeGraph : public TimeGraph
{
public:
	NNetTimeGraph
	(
		HWND    const hwndParent,
		LPCTSTR const szClass
	)
		:TimeGraph(hwndParent, szClass)
	{}

	virtual void SetModelInterface(NNetModelWriterInterface* const p)
	{
		assert(p);
		if (m_pNMWI)
			GetParams()->UnregisterObserver(*this);
		m_pNMWI = p;
		GetParams()->RegisterObserver(*this);
	}

	void SetBackgroundColorRef(COLORREF const c) override
	{
		GetParentRootWindow()->SetBackgroundColorRef(c);
	}

	void SetDefaultBackgroundColor() override 
	{ 
		SetBackgroundColorRef(D2D1::ColorF::Ivory); 
	}

protected:

	fPixel const STD_WIDTH  { 1.0_fPixel };
	fPixel const HIGH_WIDTH { 3.0_fPixel };

	NNetModelWriterInterface* m_pNMWI { nullptr };

	SignalGenerator  const * GetSigGenSelected  () const { return m_pNMWI->GetSigGenSelected(); }
	SignalGenerator        * GetSigGenSelected  ()       { return m_pNMWI->GetSigGenSelected(); }
	SigGenId                 GetSigGenIdSelected() const { return m_pNMWI->GetSigGenIdSelected(); }
	SigGenStaticData const * GetSigGenStaticData() const { return &GetSigGenSelected()->GetStaticData(); }
	NNetParameters         * GetParams          () const { return &m_pNMWI->GetParams(); }

	fPixel PaintCurve
	(
		auto                  getPoint,
		fMicroSecs      const timeStart0,
		fMicroSecs      const timeEnd,
		ID2D1SolidColorBrush* pBrush,
    	fPixel          const fPixWidth
	) const
	{
		return NNetTimeGraph::Paint
		(
			getPoint,
			timeStart0,
			timeEnd,
			GetParams()->TimeResolution(),
			pBrush,
			fPixWidth
		);
	}
};