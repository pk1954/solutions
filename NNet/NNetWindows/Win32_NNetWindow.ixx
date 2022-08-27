// Win32_NNetWindow.ixx
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>
#include "NNetModelReaderInterface.h"

export module NNetWindow;

import MoreTypes;
import D2D_DrawContext;
import Uniform2D;
import NNetController;
import GraphicsWindow;
import Sensor;

export class NNetWindow : public GraphicsWindow
{
public:
	NNetWindow() = default;
	~NNetWindow() override;

	void Start
	(
		HWND   const,
		DWORD  const,
		bool   const,
		fPixel const,
		NNetController &
	);

	void SetModelInterface(NNetModelReaderInterface* const);

	MicroMeterRect GetViewRect() const;

	DrawContext           const& GetDrawContextC() const { return m_context; }
	DrawContext& GetDrawContext() { return m_context; }
	Uniform2D<MicroMeter> const& GetCoordC() const { return m_context.GetCoordC(); }
	Uniform2D<MicroMeter>& GetCoord() { return m_context.GetCoord(); }
	MicroMeter                    PixelSize() const { return m_context.GetPixelSize(); }

	void DrawArrowsInRect(PixelRect const&, MicroMeter const) const;

	template<class CRIT>
	void DrawInteriorInRect
	(
		PixelRect const& rect,
		CRIT      const& crit
	) const
	{
		MicroMeterRect const umRect{ GetCoordC().Transform2logUnitRect(rect) };
		m_pNMRI->GetUPNobsC().Apply2AllInRectC<Nob>
			(
				umRect,
				[&](Nob const& s) { if (crit(s)) s.DrawInterior(m_context, tHighlight::normal); }
		);
	}

	template<class CRIT>
	void DrawExteriorInRect
	(
		PixelRect const& rect,
		CRIT      const& crit
	) const
	{
		MicroMeterRect const umRect{ GetCoordC().Transform2logUnitRect(rect) };
		m_pNMRI->GetUPNobsC().Apply2AllInRectC<Nob>
			(
				umRect,
				[&](Nob const& s) { if (crit(s)) s.DrawExterior(m_context, tHighlight::normal); }
		);
	}

protected:

	bool OnSize(PIXEL  const, PIXEL  const) override;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) override;

	void DrawSensors() const;
	void DrawSensorDataPoints(Sensor const* const) const;
	void DrawHighlightedSensor(Sensor const* const) const;

	NNetModelReaderInterface const* m_pNMRI{ nullptr };

	PixelPoint GetPtLast() const { return m_ptLast; }

	void SetPtLast(PixelPoint const& pt) { m_ptLast = pt; }
	void ClearPtLast() { m_ptLast.Set2Null(); }

private:
	NNetWindow(NNetWindow const&);           // noncopyable class 
	NNetWindow& operator= (NNetWindow const&) = delete;  // noncopyable class 

	NNetController* m_pController{ nullptr };
	D2D_DrawContext  m_context{ };
	fPixel           m_fPixRadiusLimit{ };
	PixelPoint       m_ptLast{ PP_NULL };	// Last cursor position during selection 
};
