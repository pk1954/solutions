// MonitorControl.ixx
//
// NNetSignals

module;

#include <dwrite.h>
#include <compare>
#include <Windows.h>

export module NNetSignals:MonitorControl;

import Types;
import PixFpDimension;
import SoundInterface;
import Direct2D;
import Signals;
import NNetModel;
import :Measurement;
import :NNetTimeGraph;

export class MonitorControl : public NNetTimeGraph
{
public:
	MonitorControl
	(
		HWND const, 
		Sound                      &,
		PixFpDimension<fMicroSecs> &,
		PixFpDimension<mV>         &,
		Observable                 &
	);

	~MonitorControl() final = default;

	void Notify(bool const) final;

	void SetDefaultBackgroundColor() override;

	void   SetModelInterface(NNetModelWriterInterface * const) final;
	LPARAM AddContextMenuEntries(HMENU const) final;

	void   Stop () final;
	void   Reset();
	bool   SignalTooHigh() const;
	float  ScaleFactor() const;
	void   ScaleSignals();
	void   StimulusTriggered();
	fPixel GetMaxSignal() const { return m_fPixMaxSignal; }

	PixelPoint GetTrackPosScreen(SignalId const, tHorzDir const) const;
	void       MoveHighlightedSignal(PIXEL const);
	void       DropSignal();

private:

	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void OnMouseLeave     () final;
	bool OnShow           (WPARAM const, LPARAM const) final;
	void OnMouseMove      (WPARAM const, LPARAM const) final;
	bool OnLButtonUp      (WPARAM const, LPARAM const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	void OnChar           (WPARAM const, LPARAM const) final { /* */ };
	void OnMouseWheel     (WPARAM const, LPARAM const) final;
	bool OnSize           (PIXEL  const, PIXEL  const) final;

	void        moveOperation   (PixelPoint const &);
	void        selectSignal    (PixelPoint const &);
	void        selectTrack     (PixelPoint const &);
	SignalNr    findSignal      (TrackNr const, PixelPoint const &) const;
	float       pixel2Track     (PIXEL const) const;
	int         findTrackPos    (PIXEL const) const;
	void        highlightSignal (SignalId const &);
	fPixel      getSignalOffset (SignalId const &) const;
	fPixel      getSignalValue  (Signal const &, fMicroSecs const) const;
	fPixelPoint getSignalPoint  (Signal const &, fMicroSecs const, fPixel const) const;
	fPixel      calcTrackHeight () const;
	fPixelPoint calcDiamondPos  () const;

	fMicroSecs pixel2scaleTime (fPixel     const fPix)    const { return GetHorzCoordC()->Transform2logUnitPos(fPix); }
	fPixel     scale2pixelTime (fMicroSecs const usScale) const { return GetHorzCoordC()->Transform2fPixelPos(usScale); }

	fMicroSecs scale2simuTime  (fMicroSecs const usScale) const { return usScale + SimulationTime::Get(); }
	fMicroSecs simu2scaleTime  (fMicroSecs const usSimu)  const { return usSimu  - SimulationTime::Get(); }

	fMicroSecs pixel2simuTime  (fPixel     const fPix)    const { return scale2simuTime(pixel2scaleTime(fPix)); }
	fPixel     simu2pixelTime  (fMicroSecs const usSimu)  const { return scale2pixelTime(simu2scaleTime(usSimu)); }

	void paintWarningRect    () const;
	void paintStimulusMarkers() const;
	void paintSignal(SignalId const &);
	void paintTrack(TrackNr const) const;
	void paintNumber(fPixel const, fPixel const, int const, D2D1::ColorF const) const;

	void PaintGraphics() final;

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	PixFpDimension<mV>& m_vertCoord;
	Sound             & m_sound;        
	MonitorData       * m_pMonitorData { nullptr };
	IDWriteTextFormat * m_pTextFormat  { nullptr };
	Observable        * m_pObservable  { nullptr };
	Measurement         m_measurement;

	ID2D1SolidColorBrush* m_pBrushNormal;
	ID2D1SolidColorBrush* m_pBrushSelected;

	TrackNr    m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL      m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	fPixel     m_fPixWinWidth       { 0.0_fPixel };
	fPixel     m_fPixRightLimit     { 0.0_fPixel };
	fPixel     m_fPixMaxSignal      { 0.0_fPixel };
};