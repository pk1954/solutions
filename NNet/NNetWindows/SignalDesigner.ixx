// SignalDesigner.ixx
//
// NNetWindows

module;

#include <array>
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
import BaseWindow;
import Scale;
import BaseScale;
import NNetModelCommands;
import NNetModel;

using std::array;
using std::wstring;
using std::unique_ptr;

export class SignalDesigner : public BaseWindow
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

	LPARAM  AddContextMenuEntries(HMENU const) final;
	wstring GetCaption()                 const final;
	void    Trigger()                          final;

	void    SetModelInterface(NNetModelWriterInterface * const);
	void    RegisterAtSigGen(SigGenId const);

private:

	inline static PIXEL const V_SCALE_WIDTH  { 35_PIXEL };
	inline static PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

	inline static PIXEL const STIMULUS_BUTTON_WIDTH  { 90_PIXEL };
	inline static PIXEL const STIMULUS_BUTTON_HEIGHT { 30_PIXEL };

	inline static D2D1::ColorF COLOR_FREQ { D2D1::ColorF::Green };

	inline static array<D2D1::ColorF, 2> COLOR_VOLT 
	{ 
		D2D1::ColorF::Blue, 
		D2D1::ColorF::Black 
	};

	void adjustLayout(PIXEL const, PIXEL const);
	void adjustWindowHeight();
	void renameSigGen();
	void scale(BaseScale * const);

	unique_ptr<SignalControl> makeSignalControl(ComputeThread const &, Observable &, Observable &);

	bool OnSize             (PIXEL const, PIXEL const)                     final;
	bool OnCommand          (WPARAM const, LPARAM const, PixelPoint const) final;
	void OnNCLButtonDblClick(WPARAM const, LPARAM const)                   final;
	void OnScaleCommand     (WPARAM const, LPARAM const)                   final;

	PixFpDimension<fMicroSecs> m_horzCoord;
	PixFpDimension<fHertz>     m_vertCoordFreq;
	PixFpDimension<mV>         m_vertCoordVolt;

	array<unique_ptr<Scale<fMicroSecs>>, 3> m_upHorzScale;
	array<unique_ptr<Scale<mV>>,         2> m_upVertScaleVolt;
	array<unique_ptr<SignalControl>,     2> m_upSignalControl;
	
	unique_ptr<Scale<fHertz>>  m_upVertScaleFreq;
	unique_ptr<SignalPreview>  m_upSignalPreview;
	
	array<unique_ptr<ArrowButton>, 2> m_upArrowButton;
	HWND                              m_hwndPreviewButton;
	unique_ptr<StimulusButton>        m_upStimulusButton;
	
	ComputeThread      const * m_pComputeThread    { nullptr };
	NNetModelWriterInterface * m_pNMWI             { nullptr };
	NNetModelCommands        * m_pCommands         { nullptr };
	HMENU                      m_hMenu             { nullptr };
	bool                       m_bIntegrated       { false };
	bool                       m_bPreview          { false };
	int                        m_iNrOfTiles        { 2 };
};