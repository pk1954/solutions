// GenerationDisplay.h
//
// Evolution

#pragma once

#include "ObserverInterface.h"
#include "win32_status.h"
#include "EvoReadBuffer.h"
#include "EvolutionCore.h"

using std::to_wstring;

class GenerationDisplay : public ObserverInterface
{
public:
	GenerationDisplay
	(
		StatusBar     * pStatusBar,
		EvoReadBuffer * pReadBuffer,
		int             iPartInStatusBar
	)
		:	m_pStatusBar      (pStatusBar),
			m_pReadBuffer     (pReadBuffer),
			m_iPartInStatusBar(iPartInStatusBar)
	{
		static PIXEL const PIX_WIDTH = PIXEL( ( static_cast<int>(wstringText.size()) + 7) * 9 );   //TODO: magic numbers
		m_pReadBuffer->RegisterObserver( this );   // notify me, if model has changed
		m_pStatusBar->CreateRectToolTip
		( 
			0, 
			& PixelRect( 0_PIXEL, 0_PIXEL, PIX_WIDTH, m_pStatusBar->GetHeight( ) ), 
			L"Generation number. Is incremented at every computed generation, not influenced by editor operations." 
		);
		m_pStatusBar->AddCustomControl( PIX_WIDTH ); 
		Notify( false );
	}

	virtual void Notify( bool const bImmediately )
	{
		EvolutionCore  const * pCore  = m_pReadBuffer->LockReadBuffer( );
		EVO_GENERATION         evoGen = pCore->GetEvoGenerationNr( );
		m_pReadBuffer->ReleaseReadBuffer( );
		m_wstrBuffer = wstringText + to_wstring( evoGen.GetValue() );
		m_pStatusBar->DisplayInPart( m_iPartInStatusBar, m_wstrBuffer );
	}

private:
	static wstring const wstringText;

	static wstring m_wstrBuffer;

	StatusBar     * m_pStatusBar;
	EvoReadBuffer * m_pReadBuffer;
	int             m_iPartInStatusBar;
};

