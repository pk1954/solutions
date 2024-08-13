// StimulusButton.cpp
//
// NNetWindows

export module NNetSignals:StimulusButton;

import Types;
import WinBasics;
import Win32_Controls;
import Resource;

export class StimulusButton
{
public:
	StimulusButton(HWND hwndParent)
	  :	m_hwndParent(hwndParent),
		m_hwndStimulusButton
		( 
			CreateButton
			(
				hwndParent,
				L" Stimulus ",
				0, 0,
				STIMULUS_BUTTON_WIDTH .GetValue(),
				STIMULUS_BUTTON_HEIGHT.GetValue(),
				IDM_TRIGGER_STIMULUS
			)
		)
	{
		BringWindowToTop(m_hwndStimulusButton);
	}

	void Enable(bool const bState)
	{
		EnableWindow(m_hwndStimulusButton, bState);
	}

	void CenterInParentWin()
	{
		PIXEL pixHorzPos 
		{ 
			(::GetWindowWidth(m_hwndParent) - STIMULUS_BUTTON_WIDTH) / 2 
		};
		::SetWindowPos
		(
			m_hwndStimulusButton,
			HWND_TOP,
			pixHorzPos.GetValue(), 10, 0, 0,
			SWP_NOSIZE
		);
	}

private:

	inline static PIXEL const STIMULUS_BUTTON_WIDTH  { 90_PIXEL };
	inline static PIXEL const STIMULUS_BUTTON_HEIGHT { 30_PIXEL };

	HWND m_hwndStimulusButton;
	HWND m_hwndParent;
};