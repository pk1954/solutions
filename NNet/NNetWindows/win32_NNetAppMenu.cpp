// win32_NNetAppMenu.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "Pipe.h"
#include "win32_util.h"
#include "NNetModelStorage.h"
#include "CommandStack.h"
#include "ComputeThread.h"
#include "AutoOpen.h"
#include "SoundInterface.h"
#include "win32_winManager.h"
#include "win32_NNetAppMenu.h"

static HMENU PopupMenu( HMENU const hMenuParent, LPCTSTR const text )
{
    HMENU hMenuPopup = CreatePopupMenu();
    AppendMenu( hMenuParent, MF_POPUP, (UINT_PTR)hMenuPopup, text );
    return hMenuPopup;
}

void NNetAppMenu::Start
( 
	HWND                     const hwndApp,
	ComputeThread    const * const pComputeThread,
	WinManager       const * const pWinManager,
	NNetModelStorage const * const pStorage,
	CommandStack     const * const pCommandStack,
	Sound            const * const pSound
) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_hwndApp        = hwndApp;
	m_pComputeThread = pComputeThread;
	m_pWinManager    = pWinManager;
	m_pStorage       = pStorage;
	m_pCommandStack  = pCommandStack;
	m_pSound         = pSound;

    SendMessage( m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NNETSIMU ) ) );
    SendMessage( m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL    ) ) );

	SetAppTitle( L"", false );

    m_hMenu = CreateMenu( );

	HBITMAP hBitmapUndo { LoadBitmap( hInstance, L"UNDO_BITMAP" ) };
	HBITMAP hBitmapRedo { LoadBitmap( hInstance, L"REDO_BITMAP" ) };

    HMENU hMenuFile = PopupMenu( m_hMenu, L"&File" );
    {
        AppendMenu( hMenuFile, MF_STRING, IDM_NEW_MODEL,     L"&New model"  );
        AppendMenu( hMenuFile, MF_STRING, IDM_OPEN_MODEL,    L"&Open model" );
        AppendMenu( hMenuFile, MF_STRING, IDM_SAVE_MODEL,    L"&Save model" );
        AppendMenu( hMenuFile, MF_STRING, IDM_SAVE_MODEL_AS, L"Save model &as" );
        AppendMenu( hMenuFile, MF_STRING, IDM_SCRIPT_DIALOG, L"&Run script" );
        AppendMenu( hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit" );
    }

    HMENU hMenuEdit = PopupMenu( m_hMenu, L"&Edit" );
    {
        HMENU hMenuSelection = PopupMenu( hMenuEdit, L"&Selection" );
        {
            AppendMenu( hMenuEdit, MF_STRING, IDM_SELECT_ALL,         L"&Select all" );
            AppendMenu( hMenuEdit, MF_STRING, IDM_SELECT_ALL_BEEPERS, L"&Select all neurons with trigger sounds" );
            AppendMenu( hMenuEdit, MF_STRING, IDM_DESELECT_ALL,       L"&Deselect all" );
        }
        AppendMenu( hMenuEdit, MF_STRING, IDM_CLEAR_BEEPERS, L"Clear all trigger sounds" );
    }

    AppendMenu( m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo );
    AppendMenu( m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo );

    HMENU hMenuAction = PopupMenu( m_hMenu, L"&Action" );
    {
        AppendMenu( hMenuAction, MF_STRING, IDM_FORWARD, L"&Proceed single step" );
        AppendMenu( hMenuAction, MF_STRING, IDM_RUN,     L"&Run" );
        AppendMenu( hMenuAction, MF_STRING, IDM_STOP,    L"&Stop" );
        HMENU hMenuAnalyze = PopupMenu( hMenuAction, L"&Analyze" );
        {
            AppendMenu( hMenuAnalyze, MF_STRING, IDM_ANALYZE_LOOPS    , L"Find &loops" );
            AppendMenu( hMenuAnalyze, MF_STRING, IDM_ANALYZE_ANOMALIES, L"Find &anomalies" );
        }
        AppendMenu( hMenuAction, MF_STRING, IDM_CENTER_MODEL, L"&Center model" );
    }

    HMENU hMenuView = PopupMenu( m_hMenu, L"&View" );
    {
        HMENU hMenuWindows = PopupMenu( hMenuView, L"&Windows" );
        {
            AppendMenu( hMenuWindows, MF_STRING, IDM_MINI_WINDOW,  L"Show &mini window" );
            AppendMenu( hMenuWindows, MF_STRING, IDM_CRSR_WINDOW,  L"Show &cursor window" );
            AppendMenu( hMenuWindows, MF_STRING, IDM_PARAM_WINDOW, L"Show &parameter window" );
            AppendMenu( hMenuWindows, MF_STRING, IDM_PERF_WINDOW,  L"Show &performance window" );
        }
        HMENU hMenuArrows = PopupMenu( hMenuView, L"&Arrows" );
        {
            AppendMenu( hMenuArrows, MF_STRING, IDD_ARROWS_ON,  L"o&n" );
            AppendMenu( hMenuArrows, MF_STRING, IDD_ARROWS_OFF, L"o&ff" );
        }
    }

    bool bRes = SetMenu( m_hwndApp, m_hMenu );
    assert( bRes );
}

void NNetAppMenu::Notify( bool const bImmediately )
{
	SetAppTitle
	( 
		m_pStorage->GetModelPath(), 
		m_pStorage->UnsavedChanges()
	);
	EnableMenuItem( m_hMenu, IDM_FORWARD, m_pComputeThread->IsRunning() ? MF_GRAYED  : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_RESET,   m_pComputeThread->IsRunning() ? MF_GRAYED  : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_RUN,     m_pComputeThread->IsRunning() ? MF_GRAYED  : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_STOP,    m_pComputeThread->IsRunning() ? MF_ENABLED : MF_GRAYED );

	EnableMenuItem( m_hMenu, IDM_CRSR_WINDOW,  m_pWinManager->IsVisible( IDM_CRSR_WINDOW  ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_MINI_WINDOW,  m_pWinManager->IsVisible( IDM_MINI_WINDOW  ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_PARAM_WINDOW, m_pWinManager->IsVisible( IDM_PARAM_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_PERF_WINDOW,  m_pWinManager->IsVisible( IDM_PERF_WINDOW  ) ? MF_GRAYED : MF_ENABLED );
	//EnableMenuItem( m_hMenu, IDM_CONS_WINDOW,  m_pWinManager->IsVisible( IDM_CONS_WINDOW  ) ? MF_GRAYED : MF_ENABLED );

	EnableMenuItem( m_hMenu, IDD_ARROWS_OFF, (Pipe::GetArrowSize() != Pipe::STD_ARROW_SIZE) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_ARROWS_ON,  (Pipe::GetArrowSize() == Pipe::STD_ARROW_SIZE) ? MF_GRAYED : MF_ENABLED );

	EnableMenuItem( m_hMenu, IDD_SOUND_ON,    m_pSound->IsOn() ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_SOUND_OFF, ! m_pSound->IsOn() ? MF_GRAYED : MF_ENABLED );

	EnableMenuItem( m_hMenu, IDD_AUTO_OPEN_ON,    AutoOpen::IsOn() ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_AUTO_OPEN_OFF, ! AutoOpen::IsOn() ? MF_GRAYED : MF_ENABLED );

	DrawMenuBar( m_hwndApp );
}

void NNetAppMenu::AdjustUndoRedo( )
{
    EnableMenuItem( m_hMenu, IDM_UNDO, m_pCommandStack->UndoStackEmpty() ? MF_GRAYED : MF_ENABLED );
    EnableMenuItem( m_hMenu, IDM_REDO, m_pCommandStack->RedoStackEmpty() ? MF_GRAYED : MF_ENABLED );
}

void NNetAppMenu::SetAppTitle( wstring const wstrAdd, bool const bUnsavedChanges )
{
    Util::SetApplicationTitle( m_hwndApp, IDS_APP_TITLE, wstrAdd + (bUnsavedChanges ? L" * " : L"") );
}
