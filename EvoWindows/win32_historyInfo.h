// win32_historyInfo.h 
//

#pragma once

#include "win32_textWindow.h"

class HistorySystem;
class ObserverInterface;

class HistInfoWindow: public TextWindow
{
public:
    HistInfoWindow( );

    void Start
	( 
		HWND const, 
		HistorySystem const *
	);

    virtual void DoPaint( );

private:
	HistorySystem const * m_pHistSystem;
};
