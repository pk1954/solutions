// win32_baseDialog.cpp
//

#include "stdafx.h"
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_rootWindow.h"
#include "win32_baseDialog.h"

using namespace std::chrono;

BaseDialog::BaseDialog( )
    : RootWindow( ),
	m_visibilityMode( tOnOffAuto::on ),
	m_visibilityCriterion( nullptr )
{ }

HWND BaseDialog::StartBaseDialog
(
    HWND                  const hwndParent,
    LPCTSTR               const lpTemplateName,
	std::function<bool()> const visibilityCriterion
)
{
	m_visibilityCriterion = visibilityCriterion;

	HWND hwnd = CreateDialogParam
    (
        nullptr,
        lpTemplateName,
        hwndParent,
        BaseDialogProc,
        ( LPARAM )this
    );

    SetWindowHandle( hwnd );

	m_visibilityMode = m_visibilityCriterion 
		               ? tOnOffAuto::automatic 
		               : IsWindowVisible( ) 
						  ? tOnOffAuto::on 
		                  : tOnOffAuto::off;

    return hwnd;
}

void BaseDialog::addWinMenu( HMENU const hMenuParent, std::wstring const strTitle ) const
{
	UINT  const STD_FLAGS = MF_BYPOSITION | MF_STRING;
	HMENU const hMenu = CreatePopupMenu();
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_AUTO, L"auto" );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_ON,   L"on"   );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_OFF,  L"off"  );
	(void)AppendMenu( hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, strTitle.c_str() );
}

void BaseDialog::adjustWinMenu( HMENU const hMenu ) const
{
	EnableMenuItem( hMenu, IDM_WINDOW_AUTO, ((m_visibilityMode == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_WINDOW_ON,   ((m_visibilityMode == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_WINDOW_OFF,  ((m_visibilityMode == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
}

void BaseDialog::contextMenu( LPARAM lParam )
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

static INT_PTR CALLBACK BaseDialogProc
( 
    HWND   const hwnd,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
	BaseDialog * const pBaseWin = reinterpret_cast<BaseDialog *>(GetWindowLongPtr( hwnd, GWLP_USERDATA ));

	switch (message)
	{

	case WM_INITDIALOG:
		(void)SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)lParam );
		return TRUE;

	case WM_RBUTTONDOWN:
		pBaseWin->SetCapture( );
		pBaseWin->SetFocus( );
		return FALSE;

	case WM_RBUTTONUP:
		(void)ReleaseCapture( );
		pBaseWin->contextMenu( lParam );
		return FALSE;

	case WM_COMMAND:
	{
		UINT const uiCmdId  = LOWORD( wParam );

		switch ( uiCmdId )
		{

		case IDM_WINDOW_ON:
			pBaseWin->m_visibilityMode = tOnOffAuto::on;
			pBaseWin->Show( TRUE );
			return FALSE;

		case IDM_WINDOW_OFF:
			pBaseWin->m_visibilityMode = tOnOffAuto::off;
			pBaseWin->Show( FALSE );
			return FALSE;

		case IDM_WINDOW_AUTO:
			pBaseWin->m_visibilityMode = tOnOffAuto::automatic;
			pBaseWin->Show( ApplyAutoCriterion( tOnOffAuto::automatic, pBaseWin->m_visibilityCriterion ) );
			return FALSE;

		default:
			break;
		}
	}
	break;

	default:
		break;
	}

	{
        BaseDialog * pBaseDialog = (BaseDialog *)GetWindowLongPtr( hwnd, GWLP_USERDATA );

        return ( RootWinIsReady( pBaseDialog ) )
            ? pBaseDialog->UserProc( message, wParam, lParam )         // normal case
            : FALSE;
    }
}
