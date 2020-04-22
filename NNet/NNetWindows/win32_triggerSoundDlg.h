// win32_triggerSoundDlg.h
//
// NNetWindows

#pragma once

#include "ObserverProxy.h"
#include "win32_baseDialog.h"

class TriggerSoundDialog
{
public:
	TriggerSoundDialog
	( 
		bool      bSoundActive ,
		Hertz     frequency,
		MilliSecs duration
	) :
		m_bSoundActive( bSoundActive ),
		m_frequency   ( frequency ),
		m_duration    ( duration )
	{};

	~TriggerSoundDialog( ) {};

	void Show( HWND const );

	bool      IsTriggerSoundActive();
	Hertz     GetFrequency();
	MilliSecs GetDuration();

private:
	bool      m_bSoundActive;
	Hertz     m_frequency;   
	MilliSecs m_duration;

	TriggerSoundDialog             ( TriggerSoundDialog const & );  // noncopyable class 
	TriggerSoundDialog & operator= ( TriggerSoundDialog const & );  // noncopyable class 

	void          handleOnOff( HWND const );
	void          initEditField( HWND const, int const, unsigned long const );
	unsigned long evaluateEditField( HWND const, int const );

	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};