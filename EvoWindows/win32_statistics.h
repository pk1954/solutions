// win32_statistics.h 
//

#pragma once

#include "EvolutionCore.h"
#include "EvoReadBuffer.h"
#include "win32_textWindow.h"

class EvoStatistics;

class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow( );
    virtual ~StatisticsWindow( );

    void Start( HWND const, EvoReadBuffer * const );
	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
	EvoReadBuffer * m_pReadBuffer;
	EvoStatistics * m_pStatistics;
};
