// SignalDesigner.h
//
// NNetWindows

#pragma once

#include "win32_scale.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "SignalControl.h"
#include "win32_graphicsWindow.h"

class NNetModelWriterInterface;
class NNetModelCommands;
class SignalGenerator;
class ComputeThread;
class Observable;

class SignalDesigner : public GraphicsWindow
{
public:
	SignalDesigner
	(
		HWND const, 
		ComputeThread const &, 
		SignalGenerator &,
		Observable &,
		NNetModelCommands &,
		NNetModelWriterInterface &
	);

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void Stop() final;

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


	inline static DESIGN m_design { DESIGN::STACKED };

	void design(PIXEL const, PIXEL const);

	void DoPaint() final;
	void OnClose() final;
	bool OnSize(PIXEL const, PIXEL const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;

	inline static Param * m_pParameters { nullptr };

	PixFpDimension<fMicroSecs>    m_horzCoord;
	PixFpDimension<fHertz>        m_vertCoordFreq;
	PixFpDimension<mV>            m_vertCoordVolt;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale1;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale2;
	unique_ptr<Scale<fHertz>>     m_upVertScaleFreq;
	unique_ptr<Scale<mV>>         m_upVertScaleVolt;
	unique_ptr<SignalControl>     m_upSignalControl1;
	unique_ptr<SignalControl>     m_upSignalControl2;
	SignalGenerator             & m_sigGen;
	NNetModelCommands           & m_commands;
	NNetModelWriterInterface    & m_nmwi;
};