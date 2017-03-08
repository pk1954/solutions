// win32_genDisplayWindow.h : 
//

#pragma once

#include "HistoryGeneration.h"
#include "win32_baseWindow.h"

class GenDisplayWindow : public BaseWindow
{
public:
    GenDisplayWindow( );
    virtual ~GenDisplayWindow( ) {};

    void StartGenDisplayWindow( HWND const );
    void DisplayGenerationBelowCursor( HIST_GENERATION const );

protected:
    virtual LRESULT UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam ) override;

private:
    HIST_GENERATION m_genDisplay;

    static COLORREF const CLR_BACK = RGB( 255, 255, 0 );
    static COLORREF const CLR_TEXT = RGB( 255, 0, 0 );
};

