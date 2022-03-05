// SignalDesigner.h
//
// NNetWindows

#pragma once

#include "win32_scale.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "SignalControl.h"
#include "win32_baseWindow.h"

class NNetModelWriterInterface;
class NNetModelCommands;
class SignalGenerator;
class ComputeThread;
class Observable;

class SignalDesigner : public BaseWindow
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

	void Stop();

private:

	inline static PIXEL const LEFT_OFFSET   { 35_PIXEL };
	inline static PIXEL const BOTTOM_OFFSET { 30_PIXEL };
	inline static PIXEL const RIGHT_OFFSET  { 35_PIXEL };
	inline static PIXEL const TOP_OFFSET    {  0_PIXEL };

	void OnClose() final;
	bool OnSize(PIXEL const, PIXEL const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;

	inline static Param * m_pParameters { nullptr };

	HWND                          m_hwndTriggerButton;
	HWND                          m_hwndApply2AllButton;
	PixFpDimension<fMicroSecs>    m_horzCoord;
	PixFpDimension<fHertz>        m_vertCoordFreq;
	PixFpDimension<mV>            m_vertCoordVolt;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale;
	unique_ptr<Scale<fHertz>>     m_upVertScaleFreq;
	unique_ptr<Scale<mV>>         m_upVertScaleVolt;
	unique_ptr<SignalControl>     m_upSignalControl;
	SignalGenerator             & m_sigGen;
	NNetModelCommands           & m_commands;
	NNetModelWriterInterface    & m_nmwi;
};