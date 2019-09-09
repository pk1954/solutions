// win32_rootWindow.cpp
//

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_refreshRateDialog.h"
#include "win32_rootWindow.h"

using namespace std::chrono;

BOOL RootWinIsReady( RootWindow const * pRootWin )
{
    return ( ( pRootWin != nullptr ) && ( pRootWin->GetWindowHandle( ) != nullptr ) );
}

RootWindow::RootWindow( ) : 
	m_hwnd( nullptr ),
	m_hwndApp( nullptr ),
    m_hTimer( nullptr ),
    m_bTimerActive( FALSE ),
    m_bDirty( TRUE ),
	m_msRefreshRate( 0ms ),
	m_visibilityMode( tOnOffAuto::on ),
	m_visibilityCriterion( nullptr )
{ }

RootWindow::~RootWindow( ) 
{ 
	deleteTimer( );
	m_hwnd   = nullptr; 
    m_hTimer = nullptr;
}

void RootWindow::StartRootWindow( std::function<bool()> const visibilityCriterion )
{
	m_visibilityCriterion = visibilityCriterion;

	m_visibilityMode = m_visibilityCriterion 
		? tOnOffAuto::automatic 
		: IsWindowVisible( ) 
		? tOnOffAuto::on 
		: tOnOffAuto::off;
}

void RootWindow::addWinMenu( HMENU const hMenuParent, std::wstring const strTitle ) const
{
	UINT  const STD_FLAGS = MF_BYPOSITION | MF_STRING;
	HMENU const hMenu = CreatePopupMenu();
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_AUTO, L"auto" );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_ON,   L"on"   );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_OFF,  L"off"  );
	(void)AppendMenu( hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, strTitle.c_str() );
}

void RootWindow::adjustWinMenu( HMENU const hMenu ) const
{
	EnableMenuItem( hMenu, IDM_WINDOW_AUTO, ((m_visibilityMode == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_WINDOW_ON,   ((m_visibilityMode == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_WINDOW_OFF,  ((m_visibilityMode == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
}

void RootWindow::contextMenu( LPARAM lParam )
{
	HMENU const hPopupMenu{ CreatePopupMenu() };
	POINT       pntPos{ GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };

	AddContextMenuEntries( hPopupMenu, pntPos );

	if ( m_visibilityCriterion )
	{
		addWinMenu( hPopupMenu, L"Show window" );
		adjustWinMenu( hPopupMenu );
	}

	if ( GetRefreshRate( ) > 0ms )
	{
		(void)AppendMenu( hPopupMenu, MF_STRING, IDM_REFRESH_RATE_DIALOG, L"Refresh Rate" );
	}

	(void)ClientToScreen( GetWindowHandle(), & pntPos );
	(void)SetForegroundWindow( GetWindowHandle( ) );

	UINT const uiID = (UINT)TrackPopupMenu
	( 
		hPopupMenu, 
		TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, 
		pntPos.x, pntPos.y, 
		0, 
		GetWindowHandle( ), 
		nullptr 
	);         	// Result is send as WM_COMMAND to this window

	if ( uiID != 0 )
		SendMessage( WM_COMMAND, uiID, lParam );

	(void)DestroyMenu( hPopupMenu );
}

void RootWindow::SetWindowHandle( HWND const hwnd ) 
{ 
	m_hwnd    = hwnd;  
	m_hwndApp = GetAncestor( m_hwnd, GA_ROOTOWNER );
};

void RootWindow::Notify( bool const bImmediately )
{
    if ( bImmediately || (m_msRefreshRate == 0ms) )
        invalidate( );
    else
    {
        m_bDirty = TRUE;
        if ( !m_bTimerActive )
        {
            m_bTimerActive = TRUE;
            invalidate( );
            startTimer( m_msRefreshRate );
        }
    }
}

void CALLBACK RootWindow::TimerProc( void * const lpParam, BOOL const TimerOrWaitFired )
{
    RootWindow * const pRootWin = reinterpret_cast<RootWindow *>( lpParam );
    if ( pRootWin->m_bDirty )
    {
        pRootWin->invalidate( );
    }
    else
    {
		pRootWin->deleteTimer( );
		pRootWin->m_bTimerActive = FALSE;
    }
}

void RootWindow::startTimer( milliseconds const msTimer )
{
	DWORD dwTime = static_cast<DWORD>(msTimer.count());
    (void)CreateTimerQueueTimer
	( 
		& m_hTimer,                     // output parameter 
		nullptr,                        // use default timer queue
		(WAITORTIMERCALLBACK)TimerProc, // the timer procedure
		static_cast<void *>( this ),    // pointer to this object as parameter to TimerProc
		dwTime,                         // timer is signaled the first time after dwTime msecs
		dwTime,                         // timer is signaled periodically every dwTime msecs
		0                               // no flags
	);
}

LRESULT RootWindow::RootWindowProc
( 
	HWND   const hwnd,
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	RootWindow * const pRootWin = reinterpret_cast<RootWindow *>(GetWindowLongPtr( hwnd, GWLP_USERDATA ));

	switch (message)
	{

	case WM_RBUTTONDOWN:
		pRootWin->SetCapture( );
		pRootWin->SetFocus( );
		return FALSE;

	case WM_RBUTTONUP:
		(void)ReleaseCapture( );
		pRootWin->contextMenu( lParam );
		return FALSE;

	case WM_COMMAND:
	{
		UINT const uiCmdId  = LOWORD( wParam );

		switch ( uiCmdId )
		{

		case IDM_WINDOW_ON:
			pRootWin->m_visibilityMode = tOnOffAuto::on;
			pRootWin->Show( TRUE );
			return FALSE;

		case IDM_WINDOW_OFF:
			pRootWin->m_visibilityMode = tOnOffAuto::off;
			pRootWin->Show( FALSE );
			return FALSE;

		case IDM_WINDOW_AUTO:
			pRootWin->m_visibilityMode = tOnOffAuto::automatic;
			pRootWin->Show( ApplyAutoCriterion( tOnOffAuto::automatic, pRootWin->m_visibilityCriterion ) );
			return FALSE;

		case IDM_REFRESH_RATE_DIALOG:
		{
			milliseconds msRefreshRateOld = GetRefreshRate( );
			milliseconds msRefreshRateNew = RefreshRateDialog::Show( hwnd, msRefreshRateOld );
			SetRefreshRate( msRefreshRateNew );
			return FALSE;
		}

		default:
			break;
		}
	}
	break;

	case WM_CLOSE:   
		pRootWin->m_visibilityMode = tOnOffAuto::off;
		break;

	default:
		break;
	}

	return pRootWin->UserProc( message, wParam, lParam );
}

