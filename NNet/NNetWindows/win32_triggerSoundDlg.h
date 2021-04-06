// win32_triggerSoundDlg.h
//
// NNetWindows

#pragma once

#include "SoundInterface.h"
#include "win32_baseDialog.h"

class TriggerSoundDialog
{
public:
	TriggerSoundDialog
	(
		Sound      const * const pSound, 
		SoundDescr const &       soundDesc 
	)
	  : m_pSound   ( pSound ),
		m_soundDesc( soundDesc )
	{};

	~TriggerSoundDialog() {};

	void Show( HWND const );

	SoundDescr const GetSound() { return m_soundDesc; }

private:
	Sound const * const m_pSound;
	SoundDescr          m_soundDesc;

	TriggerSoundDialog             ( TriggerSoundDialog const & );  // noncopyable class 
	TriggerSoundDialog & operator= ( TriggerSoundDialog const & );  // noncopyable class 

	void          onCommand( HWND const, WPARAM const, LPARAM const );
	void          handleOnOff( HWND const );
	void          initEditField( HWND const, int const, unsigned long const );
	unsigned long evaluateEditField( HWND const, int const );
	void          evaluate( HWND const );

	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};