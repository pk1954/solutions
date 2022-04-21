// win32_triggerSoundDlg.h
//
// NNetWindows

#pragma once

#include "SoundInterface.h"
#include "win32_stdDialogBox.h"

class TriggerSoundDialog : public StdDialogBox
{
public:
	TriggerSoundDialog(Sound const * const, SoundDescr const &);

	SoundDescr GetSound() const { return m_soundDesc; }

private:
	Sound const * const m_pSound;
	SoundDescr          m_soundDesc;

	TriggerSoundDialog             (TriggerSoundDialog const &) = delete;  // noncopyable class 
	TriggerSoundDialog & operator= (TriggerSoundDialog const &) = delete;  // noncopyable class 

	void          OnInitDlg(HWND const, WPARAM const, LPARAM const) final;
	bool          OnOK     (HWND const)                             final; 

	void          OnCommand(HWND const, WPARAM const, LPARAM const);
	void          handleOnOff(HWND const) const;
	unsigned long evaluateEditField(HWND const, int const) const;
	void          evaluate(HWND const);
};