// GenerationDisplay.cpp
//
// Evolution


#include "win32_baseRefreshRate.h"
#include "win32_status.h"
#include "EvolutionCore.h"
#include "GenerationDisplay.h"

using std::to_wstring;

static wstring wstringText = L"EvoGen ";

/////// inner class GenerationDisplay::RefreshRate ///////

class GenerationDisplay::RefreshRate : public BaseRefreshRate
{	
public:
	RefreshRate	
	(
		StatusBar     * pStatusBar,
		EvoReadBuffer * pReadBuffer,
		int             iPartInStatusBar
	)
	:	m_pStatusBar      (pStatusBar),
		m_pReadBuffer     (pReadBuffer),
		m_iPartInStatusBar(iPartInStatusBar)
	{}

	virtual void Trigger( )
	{
		EvolutionCore  const * pCore  = m_pReadBuffer->LockReadBuffer( );
		EVO_GENERATION         evoGen = pCore->GetEvoGenerationNr( );
		m_pReadBuffer->ReleaseReadBuffer( );
		m_wstrBuffer = wstringText + to_wstring( evoGen.GetValue() );
		m_pStatusBar->DisplayInPart( m_iPartInStatusBar, m_wstrBuffer );
	}

private:
	wstring         m_wstrBuffer;
	StatusBar     * m_pStatusBar;
	EvoReadBuffer * m_pReadBuffer;
	int             m_iPartInStatusBar;
};

/////// functions of class GenerationDisplay ///////

using namespace std::chrono;

GenerationDisplay::GenerationDisplay
(
	StatusBar     * pStatusBar,
	EvoReadBuffer * pReadBuffer,
	int             iPartInStatusBar
)
	:	m_pRefreshRate( nullptr )
{
	static PIXEL const PIX_WIDTH = PIXEL( ( static_cast<int>(wstringText.size()) + 7) * 9 );   //TODO: avoid magic numbers
	m_pRefreshRate = new RefreshRate
	( 
		pStatusBar,
		pReadBuffer,
		iPartInStatusBar
	);
	m_pRefreshRate->SetRefreshRate( 300ms );
	pReadBuffer->RegisterObserver( this );   // notify me, if model has changed
	pStatusBar->AddCustomControl( PIX_WIDTH ); 
	Notify( false );
	pStatusBar->CreateRectToolTip
	( 
		0, 
		& PixelRect( 0_PIXEL, 0_PIXEL, PIX_WIDTH, pStatusBar->GetHeight( ) ), 
		L"Generation number. Is incremented at every computed generation, not influenced by editor operations." 
	);
}

void GenerationDisplay::Notify( bool const bImmediately )
{
	m_pRefreshRate->Notify( bImmediately );
}
