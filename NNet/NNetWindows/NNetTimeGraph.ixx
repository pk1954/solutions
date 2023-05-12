// NNetTimeGraph.ixx
//
// NNetWindows

module;

#include <cassert>
#include <math.h>
#include <Windows.h>

export module NNetWin32:NNetTimeGraph;

import Types;
import PixFpDimension;
import Direct2D;
import TimeGraph;
import NNetModel;

export class NNetTimeGraph : public TimeGraph
{
public:
	NNetTimeGraph
	(
		HWND                   const hwndParent,
		PixFpDimension<fMicroSecs>* pHorzCoord
	)
		:TimeGraph(hwndParent, pHorzCoord)
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

	fPixel const STD_WIDTH { 1.0_fPixel };
	fPixel const HIGH_WIDTH { 3.0_fPixel };

	NNetModelWriterInterface* m_pNMWI { nullptr };

	SignalGenerator  const * GetSigGenSelected  () const { return m_pNMWI->GetSigGenSelected(); }
	SignalGenerator        * GetSigGenSelected  ()       { return m_pNMWI->GetSigGenSelected(); }
	SigGenId                 GetSigGenIdSelected() const { return m_pNMWI->GetSigGenIdSelected(); }
	SigGenStaticData const * GetSigGenStaticData() const { return &GetSigGenSelected()->GetStaticData(); }
	NNetParameters         * GetParams          () const { return &m_pNMWI->GetParams(); }

	fPixel PaintCurve
	(
		auto               getPoint,
		fMicroSecs   const timeStart0,
		fMicroSecs   const timeEnd,
		D2D1::ColorF const color,
		fPixel       const fPixWidth
	) const
	{
		return NNetTimeGraph::Paint
		(
			getPoint,
			timeStart0,
			timeEnd,
			GetParams()->TimeResolution(),
			color,
			fPixWidth
		);
	}
};