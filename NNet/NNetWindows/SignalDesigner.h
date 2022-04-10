// SignalDesigner.h
//
// NNetWindows

#pragma once

#include "win32_scale.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "SignalControl.h"
#include "SignalPreview.h"
#include "win32_graphicsWindow.h"

class NNetModelCommands;
class SignalGenerator;
class ComputeThread;
class Observable;

class SignalDesigner : public GraphicsWindow
{
public:
	void Initialize
	(
		HWND const, 
		ComputeThread const &, 
		Observable &,
		NNetModelCommands *
	);

	void Stop() final;

	void SetModelInterface(NNetModelWriterInterface * const p)
	{
		assert(p);
		if (m_pNMWI)
			m_pNMWI->UnregisterSigGenActiveObserver(*this);
		m_upSignalControl1->SetModelInterface(p);
		m_upSignalControl2->SetModelInterface(p);
		m_upSignalPreview ->SetModelInterface(p);
		m_pNMWI = p;
		m_pNMWI->RegisterSigGenActiveObserver(*this);
	}

	wstring GetTitle() const final;

	enum class DESIGN { INTEGRATED, STACKED };

	static DESIGN GetDesign() 
	{ 
		return m_design; 
	};

	static void SetDesign(DESIGN const d) 
	{ 
		m_design = d; 
	}

	static void ToggleDesign() 
	{ 
		SetDesign((m_design == DESIGN::INTEGRATED) ? DESIGN::STACKED : DESIGN::INTEGRATED);
	}

private:

	inline static PIXEL const V_SCALE_WIDTH  { 35_PIXEL };
	inline static PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

	inline static D2D1::ColorF COLOR_FREQ { D2D1::ColorF::Green };
	inline static D2D1::ColorF COLOR_VOLT { D2D1::ColorF::Blue  };

	inline static DESIGN m_design   { DESIGN::STACKED };
	inline static bool   m_bPreview { true };

	void design(PIXEL const, PIXEL const);
	unique_ptr<SignalControl> makeSignalControl(ComputeThread const &, Observable &);

	void DoPaint() final;
	bool OnSize(PIXEL const, PIXEL const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const) final;

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
	NNetModelWriterInterface    * m_pNMWI     { nullptr };
	NNetModelCommands           * m_pCommands { nullptr };
	HMENU                         m_hMenu     { nullptr };
};