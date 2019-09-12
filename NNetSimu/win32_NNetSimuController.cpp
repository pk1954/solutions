// win32_NNetSimuController.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "win32_aboutBox.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetSimuWindow.h"
#include "win32_NNetSimuController.h"

NNetSimuController::NNetSimuController() :
	m_pAppWindow              ( nullptr ),
	m_pNNetWorkThreadInterface( nullptr ),
	m_pWinManager             ( nullptr ),
    m_pDelay                  ( nullptr ),
	m_pAppMenu                ( nullptr ),
	m_pStatusBar              ( nullptr ),
	m_hCrsrWait               ( 0 )
{ }

NNetSimuController::~NNetSimuController( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pAppWindow               = nullptr;
	m_pWinManager              = nullptr;
	m_pDelay                   = nullptr;
    m_pStatusBar               = nullptr;
	m_pAppMenu                 = nullptr;
}

void NNetSimuController::Initialize
( 
	NNetSimuWindow          * const pAppWindow,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	WinManager              * const pWinManager,
	Delay                   * const pDelay,
	NNetSimuMenu            * const pAppMenu
)
{
	Controller::Initialize( pAppWindow,  pNNetWorkThreadInterface );
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
	m_pAppWindow               = pAppWindow;
	m_pWinManager              = pWinManager;
    m_pDelay                   = pDelay;
	m_pAppMenu                 = pAppMenu;
	m_hCrsrWait                = LoadCursor( NULL, IDC_WAIT );
}

bool NNetSimuController::processUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

void NNetSimuController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
	
	if ( Controller::ProcessCommand( wmId, lParam ) )  // Some commands are handled by 
		return;                                        // the framework controller

	if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                             // but do not concern the model

    switch (wmId)
    {

		default:
			assert( false );
	        break;
    }
}
