// SignalDesigner.ixx
//
// NNetSignals

module;

#include <array>
#include <string>
#include <memory>
#include <compare>
#include <Windows.h>

export module NNetSignals:SignalDesigner;

import Types;
import Observable;
import PixFpDimension;
import ArrowButton;
import Direct2D;
import BaseWindow;
import Scale;
import BaseScale;
import NNetModel;
import :SimuRunning;
import :SignalControl;
import :SignalPreview;
import :StimulusButton;

using std::array;
using std::wstring;
using std::unique_ptr;

export class SignalDesigner : public BaseWindow
{
public:
	void Initialize
	(
		HWND const, 
		SimuRunning const&,
		Observable &,
		Observable &
	);

	COLORREF GetBackgroundColorRef() const final { return m_upSignalPreview->GetBackgroundColorRef(); }

	void    SetBackgroundColorRef(COLORREF const) final;
	LPARAM  AddContextMenuEntries(HMENU const)    final;
	wstring GetCaption()                    const final;
	void    Trigger()                             final;
	void    SetGrid(bool const, bool const)       final;

	bool    HasScales() const final { return m_upSignalControl[0]->HasScales(); }
	bool    HasGrid  () const final { return m_upSignalControl[0]->HasGrid(); }

	void    SetModelInterface(NNetModelWriterInterface * const);
	void    RegisterAtSigGen(SigGenId const);

	static void AddSigGenMenu(HMENU const, SigGenId const);

private:

	inline static PIXEL const V_SCALE_WIDTH  { 35_PIXEL };
	inline static PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

	inline static PIXEL const STIMULUS_BUTTON_WIDTH  { 90_PIXEL };
	inline static PIXEL const STIMULUS_BUTTON_HEIGHT { 30_PIXEL };

	inline static Color COLOR_FREQ { D2D1::ColorF::Green };

	inline static array<Color, 2> COLOR_VOLT 
	{ 
		D2D1::ColorF::Blue, 
		D2D1::ColorF::Black 
	};

	void adjustLayout(PIXEL const, PIXEL const);
	void adjustWindowHeight();
	void scale(BaseScale * const);

	unique_ptr<SignalControl>     makeSignalControl(Observable &, Observable &);
	unique_ptr<Scale<fMicroSecs>> makeHorzScale();

	bool OnSize        (PIXEL const, PIXEL const)                     final;
	bool OnCommand     (WPARAM const, LPARAM const, PixelPoint const) final;
	void OnScaleCommand(WPARAM const, LPARAM const)                   final;

	PixFpDimension<fMicroSecs> m_horzCoord;
	PixFpDimension<fHertz>     m_vertCoordFreq;
	PixFpDimension<mV>         m_vertCoordVolt;

	unique_ptr<Scale<fHertz>>  m_upVertScaleFreq;
	unique_ptr<SignalPreview>  m_upSignalPreview;

	array<unique_ptr<Scale<fMicroSecs>>, 3> m_upHorzScale;
	array<unique_ptr<Scale<mV>>,         2> m_upVertScaleVolt;
	array<unique_ptr<SignalControl>,     2> m_upSignalControl; // create first, destroy last
	
	array<unique_ptr<ArrowButton>, 2> m_upArrowButton;
	HWND                              m_hwndPreviewButton { nullptr };
	unique_ptr<StimulusButton>        m_upStimulusButton;
	
	SimuRunning        const * m_pSimuRunning   { nullptr };
	NNetModelWriterInterface * m_pNMWI          { nullptr };
	HMENU                      m_hMenu          { nullptr };
	bool                       m_bIntegrated    { false };
	bool                       m_bPreview       { false };
	int                        m_iNrOfTiles     { 2 };
	float                      m_fGridDimFactor { 0.0f };
	Observable                 m_gridObservable;
};