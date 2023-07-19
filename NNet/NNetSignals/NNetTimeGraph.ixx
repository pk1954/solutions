// NNetTimeGraph.ixx
//
// NNetWindows

module;

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

export class NNetTimeGraph : public TimeGraph
{
public:
	NNetTimeGraph(HWND const hwndParent)
		:TimeGraph(hwndParent)
	{}

	virtual void SetModelInterface(NNetModelWriterInterface* const p)
	{
		assert(p);
		if (m_pNMWI)
			GetParams()->UnregisterObserver(*this);
		m_pNMWI = p;
		GetParams()->RegisterObserver(*this);
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