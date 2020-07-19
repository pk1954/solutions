// win32_triggerSoundDlg.h
//
// NNetWindows

#pragma once

#include "win32_sound.h"
#include "win32_baseDialog.h"

class TriggerSoundDialog
{
public:
	TriggerSoundDialog( SoundDescr const & sound )
	  : m_sound( sound )
	{};

	~TriggerSoundDialog( ) {};

	void Show( HWND const );

	SoundDescr const GetSound() { return m_sound; }

private:
	SoundDescr m_sound;

	TriggerSoundDialog             ( TriggerSoundDialog const & );  // noncopyable class 
	TriggerSoundDialog & operator= ( TriggerSoundDialog const & );  // noncopyable class 

	void          onCommand( HWND const, WPARAM const, LPARAM const );
	void          handleOnOff( HWND const );
	void          initEditField( HWND const, int const, unsigned long const );
	unsigned long evaluateEditField( HWND const, int const );
	void          evaluate( HWND const );

	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};