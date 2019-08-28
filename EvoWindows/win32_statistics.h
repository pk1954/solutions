// win32_statistics.h 
//

#pragma once

#include "win32_textWindow.h"

class ReadBuffer;
class EvoStatistics;

class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow( );
    virtual ~StatisticsWindow( );

    void Start( HWND const, ReadBuffer * const );
	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
	ReadBuffer    * m_pReadBuffer;
	EvoStatistics * m_pStatistics;
};
