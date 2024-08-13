// MonitorWindow.ixx
//
// NNetSignals

export module NNetSignals:MonitorWindow;

import std;
import Types;
import Scale;
import Observable;
import PixFpDimension;
import BaseWindow;
import SoundInterface;
import NNetModel;
import :SimuRunning;
import :MonitorControl;
import :StimulusButton;

using std::unique_ptr;
using std::wstring;

export class MonitorWindow : public BaseWindow
{
public:

	MonitorWindow();
	~MonitorWindow() final;

	void Start(HWND const, SimuRunning const&, Sound&, Observable&);
	void SetModelInterface(NNetModelWriterInterface* const) const;
	void ResetHorzCoord();

	void SetHighSigObservable(Observable &obs) { m_upMonitorControl->SetHighSigObservable(obs); }

	PixelPoint GetTrackPosScreen(SignalId const id, tHorzDir const dir)  const 
	{ 
		return m_upMonitorControl->GetTrackPosScreen(id, dir); 
	}

	COLORREF GetBackgroundColorRef() const final
	{
		return m_upMonitorControl->GetBackgroundColorRef();
	}

	void SetBackgroundColorRef(COLORREF const c) final
	{ 
		m_upMonitorControl->SetBackgroundColor(Color(c)); 
	}

	void     MoveHighlightedSignal(PIXEL const pix)  const { m_upMonitorControl->MoveHighlightedSignal(pix); }
	void     DropSignal           ()                 const { m_upMonitorControl->DropSignal(); }
	bool     IsAnySignalHighlighted()                const { return m_upMonitorControl->IsAnySignalHighlighted(); }
	bool	 IsSignalHighlighted(SignalId const& id) const { return m_upMonitorControl->IsSignalHighlighted(id); }
	SignalId SetHighlightedSignal(SignalId const id)       { return m_upMonitorControl->SetHighlightedSignal(id); }
	SignalId SetHighlightedSignal(Signal   const &s)       { return m_upMonitorControl->SetHighlightedSignal(s); }
	SignalId GetHighlightedSignalId()                const { return m_upMonitorControl->GetHighlightedSignalId(); }

	PixFpDimension<fMicroSecs> & HorzCoord() { return m_horzCoord; }
	PixFpDimension<mV>         & VertCoord() { return m_vertCoord; }
	bool IsHorzScaleLocked()           const { return m_upHorzScale->IsScaleLocked(); }
	void SetHorzScaleLocked(bool const b)    { m_upHorzScale->SetBlock2Zero(b); }

private:
	inline static PIXEL const RIGHT_BORDER   { 25_PIXEL };
	inline static PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

	inline static fMicroSecs const DEFAULT_PIXEL_SIZE{ 100.0_MicroSecs };

	bool OnCommand   (WPARAM const, LPARAM const, PixelPoint const) final;
	void OnPaint     ()                                             final;
	bool OnMove      (PIXEL  const, PIXEL  const)                   final;
	bool OnSize      (PIXEL  const, PIXEL  const)                   final;
	bool OnMouseWheel(WPARAM const, LPARAM const)                   final;

	wstring GetCaption() const final;

	Sound                       * m_pSound;        
	PixFpDimension<fMicroSecs>    m_horzCoord;
	PixFpDimension<mV>            m_vertCoord;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale;
	unique_ptr<MonitorControl>    m_upMonitorControl;
	unique_ptr<StimulusButton>    m_upStimulusButton;
	Observable                  * m_pMoveSizeObservable { nullptr };
	SimuRunning           const * m_pSimuRunning        { nullptr };;
};
