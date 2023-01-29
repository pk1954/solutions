// SignalDesigner.ixx
//
// NNetWindows

module;

#include <string>
#include <memory>
#include <compare>
#include <Windows.h>

export module SignalDesigner;

import Types;
import Observable;
import PixFpDimension;
import StimulusButton;
import SignalControl;
import ComputeThread;
import ArrowButton;
import SignalPreview;
import Direct2D;
import GraphicsWindow;
import Scale;
import NNetModelCommands;
import NNetModel;

using std::wstring;
using std::unique_ptr;

export class SignalDesigner : public GraphicsWindow
{
public:
	void Initialize
	(
		HWND const, 
		ComputeThread const &, 
		Observable &,
		Observable &,
		NNetModelCommands *
	);

	void Stop() final;

	LPARAM AddContextMenuEntries(HMENU const) final;

	void SetModelInterface(NNetModelWriterInterface * const);

	wstring GetCaption() const final;

	void RegisterAtSigGen(SigGenId const);

	enum class DESIGN { INTEGRATED, STACKED };

	DESIGN GetDesign() const { return m_design; };

private:

	inline static PIXEL const V_SCALE_WIDTH  { 35_PIXEL };
	inline static PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

	inline static PIXEL const STIMULUS_BUTTON_WIDTH  { 90_PIXEL };
	inline static PIXEL const STIMULUS_BUTTON_HEIGHT { 30_PIXEL };

	inline static D2D1::ColorF COLOR_FREQ { D2D1::ColorF::Green };
	inline static D2D1::ColorF COLOR_VOLT { D2D1::ColorF::Blue  };

	void renameSigGen();
	void toggleDesign();
	void design(PIXEL const, PIXEL const);
	unique_ptr<SignalControl> makeSignalControl(ComputeThread const &, Observable &, Observable &);

	void DoPaint            ()                                             final;
	bool OnSize             (PIXEL const, PIXEL const)                     final;
	bool OnCommand          (WPARAM const, LPARAM const, PixelPoint const) final;
	void OnNCLButtonDblClick(WPARAM const, LPARAM const)                   final;
	void OnScaleCommand     (WPARAM const, LPARAM const)                   final;

	PixFpDimension<fMicroSecs>    m_horzCoord;
	PixFpDimension<fHertz>        m_vertCoordFreq;
	PixFpDimension<mV>            m_vertCoordVolt1;
	PixFpDimension<mV>            m_vertCoordVolt2;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale1;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale2;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale3;
	unique_ptr<Scale<fHertz>>     m_upVertScaleFreq;
	unique_ptr<Scale<mV>>         m_upVertScaleVolt1;
	unique_ptr<Scale<mV>>         m_upVertScaleVolt2;
	unique_ptr<SignalControl>     m_upSignalControl1;
	unique_ptr<SignalControl>     m_upSignalControl2;
	unique_ptr<SignalPreview>     m_upSignalPreview;
	unique_ptr<ArrowButton>       m_upArrowButton;
	unique_ptr<StimulusButton>    m_upStimulusButton;
	ComputeThread         const * m_pComputeThread { nullptr };
	NNetModelWriterInterface    * m_pNMWI          { nullptr };
	NNetModelCommands           * m_pCommands      { nullptr };
	HMENU                         m_hMenu          { nullptr };
	DESIGN                        m_design         { DESIGN::STACKED };
	bool                          m_bPreview       { true };
};