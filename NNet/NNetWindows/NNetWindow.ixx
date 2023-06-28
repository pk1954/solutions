// NNetWindow.ixx
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>

export module NNetWin32:NNetWindow;

import Types;
import Scale;
import Uniform2D;
import D2D_DrawContext;
import GraphicsWindow;
import NNetModel;
import :NNetController;

using std::unique_ptr;

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
		NNetController &,
		MonitorWindow const *
	);

	void SetModelInterface(NNetModelReaderInterface* const);

	MicroMeterRect GetViewRect() const;

	DrawContext           const & GetDrawContextC() const { return m_context; }
	DrawContext                 & GetDrawContext()        { return m_context; }
	Uniform2D<MicroMeter> const & GetCoordC()       const { return m_context.GetCoordC(); }
	Uniform2D<MicroMeter>       & GetCoord()              { return m_context.GetCoord(); }
	MicroMeter                    PixelSize()       const { return m_context.GetPixelSize(); }

	void DrawArrowsInRect(PixelRect const&, MicroMeter const) const;
	//void DrawMicroSensorsInRect(PixelRect const&) const;

	SignalId FindSignalHandle(MicroMeterPnt const&) const;

	template<class CRIT>
	void DrawInteriorInRect
	(
		PixelRect const& rect,
		CRIT      const& crit
	) const
	{
		MicroMeterRect const umRect { GetCoordC().Transform2logUnitRect(rect) };
		m_pNMRI->GetUPNobsC().Apply2AllInRectC<Nob>
		(
			umRect,
			[&](Nob const& s) 
			{ 
				if (crit(s)) 
					s.DrawInterior(m_context, tHighlight::normal); 
			}
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
			[&](Nob const& s) 
			{ 
				if (crit(s)) 
					s.DrawExterior(m_context, tHighlight::normal); 
			}
		);
	}

protected:

	ID2D1SolidColorBrush* m_pBrushSensorNormal   { nullptr };
	ID2D1SolidColorBrush* m_pBrushSensorSelected { nullptr };

	D2D_DrawContext m_context { };

	bool OnSize   (PIXEL  const, PIXEL  const)                   override;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) override;

	void DrawSensors() const;

	NNetModelReaderInterface const* m_pNMRI          { nullptr };
	MonitorWindow            const* m_pMonitorWindow { nullptr };

	PixelPoint GetPtLast() const { return m_ptLast; }

	void SetPtLast(PixelPoint const& pt) { m_ptLast = pt; }
	void ClearPtLast() { m_ptLast.Set2Null(); }

private:
	NNetWindow(NNetWindow const&);                       // noncopyable class 
	NNetWindow& operator= (NNetWindow const&) = delete;  // noncopyable class 

	inline static fPixel const HRADIUS { 20._fPixel };
	inline static fPixel const VRADIUS { 10._fPixel };

	NNetController * m_pController     { nullptr };
	fPixel           m_fPixRadiusLimit { };
	PixelPoint       m_ptLast          { PP_NULL };	// Last cursor position during selection 

	void drawSignalCable(SignalId const&, MicroMeterPnt  const&, ID2D1SolidColorBrush&) const;
};
