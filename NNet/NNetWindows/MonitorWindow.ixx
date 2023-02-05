// MonitorWindow.ixx
//
// NNetWindows

module;

#include <string>
#include <memory>
#include <Windows.h>

export module MonitorWindow;

import Types;
import Scale;
import Observable;
import PixFpDimension;
import BaseWindow;
import ComputeThread;
import MonitorControl;
import StimulusButton;
import SoundInterface;
import NNetModelCommands;
import NNetModel;

using std::unique_ptr;
using std::wstring;

export class MonitorWindow : public BaseWindow
{
public:

	MonitorWindow();
	~MonitorWindow() final;

	void Start(HWND const, ComputeThread const &, Sound &, NNetModelCommands &, Observable &);
	void SetModelInterface(NNetModelWriterInterface* const) const;
	void StimulusTriggered() const;
	void ResetHorzCoord();

	fPixel GetTrackPos(TrackNr const nr) const { return m_upMonitorControl->GetTrackPos(nr); }

	PixFpDimension<fMicroSecs> const& HorzCoord() const { return m_horzCoord; }
	PixFpDimension<mV>         const& VertCoord() const { return m_vertCoord; }

private:
	inline static PIXEL const RIGHT_BORDER   { 25_PIXEL };
	inline static PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

	inline static fMicroSecs const DEFAULT_PIXEL_SIZE{ 100.0_MicroSecs };

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnPaint  ()                                             final;
	bool OnMove   (PIXEL  const, PIXEL const)                    final;
	bool OnSize   (PIXEL  const, PIXEL const)                    final;

	wstring GetCaption() const final;

	PixFpDimension<fMicroSecs>    m_horzCoord;
	PixFpDimension<mV>            m_vertCoord;
	ComputeThread         const * m_pComputeThread { nullptr };
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale;
	unique_ptr<MonitorControl>    m_upMonitorControl;
	unique_ptr<StimulusButton>    m_upStimulusButton;
	Observable                  * m_pMoveSizeObservable;
};
