// win32_statistics.h 
//

#pragma once

#include "EvolutionCore.h"
#include "win32_readBuffer.h"
#include "win32_textWindow.h"

class EvoStatistics;

class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow( );
    virtual ~StatisticsWindow( );

    void Start( HWND const, ReadBuffer<EvolutionCore> * const );
	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
	ReadBuffer<EvolutionCore> * m_pReadBuffer;
	EvoStatistics             * m_pStatistics;
};
