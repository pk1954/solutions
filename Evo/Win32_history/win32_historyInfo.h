// win32_historyInfo.h 
//
// Win32_history

#pragma once

#include "win32_textWindow.h"

class HistorySystem;

class HistInfoWindow: public TextWindow
{
public:
    HistInfoWindow( );

	void Start(	HWND const, function<bool()> const );
	void Stop( );

	void SetHistorySystem( HistorySystem * );

    virtual void DoPaint( TextBuffer & );

private:
	HistorySystem * m_pHistSys;
};
