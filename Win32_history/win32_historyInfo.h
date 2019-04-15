// win32_historyInfo.h 
//

#pragma once

#include "win32_textWindow.h"

class HistorySystem;

class HistInfoWindow: public TextWindow
{
public:
    HistInfoWindow( );

    void Start
	( 
		HWND const, 
		HistorySystem const *
	);

	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
	HistorySystem const * m_pHistSystem;
};
