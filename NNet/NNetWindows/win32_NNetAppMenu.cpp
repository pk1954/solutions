// win32_NNetAppMenu.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "Pipe.h"
#include "win32_util.h"
#include "CommandStack.h"
#include "ComputeThread.h"
#include "AutoOpen.h"
#include "SoundInterface.h"
#include "win32_winManager.h"
#include "win32_NNetAppMenu.h"

HMENU NNetAppMenu::popupMenu( HMENU const hMenuParent, LPCTSTR const text )
{
    HMENU hMenuPopup = CreatePopupMenu();
    AppendMenu( hMenuParent, MF_POPUP, (UINT_PTR)hMenuPopup, text );
    return hMenuPopup;
}

NNetAppMenu::NNetAppMenu( )
  : m_pOnOffArrows   ( new OnOffPair( this, IDD_ARROWS_ON,      IDD_ARROWS_OFF     ) ),
    m_pOnOffSound    ( new OnOffPair( this, IDD_SOUND_ON,       IDD_SOUND_OFF      ) ),
    m_pOnOffAutoOpen ( new OnOffPair( this, IDD_AUTO_OPEN_ON,   IDD_AUTO_OPEN_OFF  ) ),
    m_pOnOffCmdCmbine( new OnOffPair( this, IDD_CMD_COMBINE_ON, IDD_CMD_COMBINE_OFF) )
{ }

void NNetAppMenu::Start
( 
	HWND                  const hwndApp,
	ComputeThread const * const pComputeThread,
	WinManager    const * const pWinManager,
	CommandStack  const * const pCommandStack,
	Sound         const * const pSound
) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_hwndApp        = hwndApp;
	m_pComputeThread = pComputeThread;
	m_pWinManager    = pWinManager;
	m_pCommandStack  = pCommandStack;
	m_pSound         = pSound;

    SendMessage( m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NNETSIMU ) ) );
    SendMessage( m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL    ) ) );

    m_hMenu = CreateMenu( );

	HBITMAP hBitmapUndo { LoadBitmap( hInstance, L"UNDO_BITMAP" ) };
	HBITMAP hBitmapRedo { LoadBitmap( hInstance, L"REDO_BITMAP" ) };

    HMENU hMenuFile = popupMenu( m_hMenu, L"&File" );
    {
        AppendMenu( hMenuFile, MF_STRING, IDM_NEW_MODEL,     L"&New model"  );
        AppendMenu( hMenuFile, MF_STRING, IDM_OPEN_MODEL,    L"&Open model" );
        AppendMenu( hMenuFile, MF_STRING, IDM_SAVE_MODEL,    L"&Save model" );
        AppendMenu( hMenuFile, MF_STRING, IDM_SAVE_MODEL_AS, L"Save model &as" );
        AppendMenu( hMenuFile, MF_STRING, IDM_SCRIPT_DIALOG, L"&Run script" );
        AppendMenu( hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit" );
    }

    HMENU hMenuEdit = popupMenu( m_hMenu, L"&Edit" );
    {
        HMENU hMenuSelection = popupMenu( hMenuEdit, L"&Selection" );
        {
            AppendMenu( hMenuEdit, MF_STRING, IDM_SELECT_ALL,         L"&Select all" );
            AppendMenu( hMenuEdit, MF_STRING, IDM_SELECT_ALL_BEEPERS, L"&Select all neurons with trigger sounds" );
            AppendMenu( hMenuEdit, MF_STRING, IDM_DESELECT_ALL,       L"&Deselect all" );
        }
        AppendMenu( hMenuEdit, MF_STRING, IDM_CLEAR_BEEPERS, L"Clear all trigger sounds" );
    }

    AppendMenu( m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo );
    AppendMenu( m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo );

    HMENU hMenuAction = popupMenu( m_hMenu, L"&Action" );
    {
        AppendMenu( hMenuAction, MF_STRING, IDM_FORWARD, L"&Proceed single step" );
        AppendMenu( hMenuAction, MF_STRING, IDM_RUN,     L"&Run" );
        AppendMenu( hMenuAction, MF_STRING, IDM_STOP,    L"&Stop" );
        HMENU hMenuAnalyze = popupMenu( hMenuAction, L"&Analyze" );
        {
            AppendMenu( hMenuAnalyze, MF_STRING, IDM_ANALYZE_LOOPS    , L"Find &loops" );
            AppendMenu( hMenuAnalyze, MF_STRING, IDM_ANALYZE_ANOMALIES, L"Find &anomalies" );
        }
        AppendMenu( hMenuAction, MF_STRING, IDM_CENTER_MODEL, L"&Center model" );
    }

    HMENU hMenuView = popupMenu( m_hMenu, L"&View" );
    {
        HMENU hMenuWindows = popupMenu( hMenuView, L"&Windows" );
        {
            AppendMenu( hMenuWindows, MF_STRING, IDM_MINI_WINDOW,  L"Show &mini window" );
            AppendMenu( hMenuWindows, MF_STRING, IDM_CRSR_WINDOW,  L"Show &cursor window" );
            AppendMenu( hMenuWindows, MF_STRING, IDM_PARAM_WINDOW, L"Show &parameter window" );
            AppendMenu( hMenuWindows, MF_STRING, IDM_PERF_WINDOW,  L"Show &performance window" );
        }
        m_pOnOffArrows->appendOnOffMenu( hMenuView, L"&Arrows" );
    }
    HMENU hMenuOptions = popupMenu( m_hMenu, L"&Options" );
    {
        m_pOnOffSound    ->appendOnOffMenu( hMenuOptions, L"&Sound" );
        m_pOnOffAutoOpen ->appendOnOffMenu( hMenuOptions, L"Auto&Open" );
        m_pOnOffCmdCmbine->appendOnOffMenu( hMenuOptions, L"&CombUndoRedo" );
    }
    HMENU hMenuHelp = popupMenu( m_hMenu, L"&Help" );
    {
        AppendMenu( hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info..." );
    }

    bool bRes = SetMenu( m_hwndApp, m_hMenu );
    assert( bRes );
}

void NNetAppMenu::enable( unsigned int const id, bool const bCrit )
{
    EnableMenuItem( m_hMenu, id, bCrit ? MF_GRAYED : MF_ENABLED );
}

void NNetAppMenu::Notify( bool const bImmediately )
{
    enable( IDM_FORWARD, m_pComputeThread->IsRunning() );
    enable( IDM_RESET,   m_pComputeThread->IsRunning() );
    enable( IDM_RUN,     m_pComputeThread->IsRunning() );
    enable( IDM_STOP,  ! m_pComputeThread->IsRunning() );

    enable( IDM_CRSR_WINDOW,  m_pWinManager->IsVisible( IDM_CRSR_WINDOW  ) );
    enable( IDM_MINI_WINDOW,  m_pWinManager->IsVisible( IDM_MINI_WINDOW  ) );
    enable( IDM_PARAM_WINDOW, m_pWinManager->IsVisible( IDM_PARAM_WINDOW ) );
    enable( IDM_PERF_WINDOW,  m_pWinManager->IsVisible( IDM_PERF_WINDOW  ) );

    m_pOnOffArrows   ->enableOnOff( Pipe::GetArrowSize() == Pipe::STD_ARROW_SIZE );
    m_pOnOffSound    ->enableOnOff( m_pSound->IsOn() );
    m_pOnOffAutoOpen ->enableOnOff( AutoOpen::IsOn() );
    m_pOnOffCmdCmbine->enableOnOff( m_pCommandStack->GetCombineCmdsFlag() );

    DrawMenuBar( m_hwndApp );
}

void NNetAppMenu::AdjustUndoRedo( )
{
    enable( IDM_UNDO, m_pCommandStack->UndoStackEmpty() );
    enable( IDM_REDO, m_pCommandStack->RedoStackEmpty() );
}
