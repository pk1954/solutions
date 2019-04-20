// win32_resetDialog.cpp
//
// Evolution

#include "stdafx.h"
#include "Resource.h"
#include "GridDimensions.h"
#include "win32_util.h"
#include "win32_resetDlg.h"

static int const BUFLEN = 20;
static wchar_t   m_wBuffer[BUFLEN];
static int       m_iResult;
static int       m_iWidth;
static int       m_iHeight;
static int       m_iGridType;

int ResetDialog::GetNewWidth( )
{
	return m_iWidth;
}

int ResetDialog::GetNewHeight()
{
	return m_iHeight;
}

int ResetDialog::GetNewNrOfNeighbors()
{
	return (m_iGridType == IDM_GRID_TYPE_HEX) ? 6 : 8;
}

int ResetDialog::Show( HWND const hwndParent )
{
	m_iResult = -1;

	DialogBox( nullptr, MAKEINTRESOURCE(IDM_RESET_DIALOG), hwndParent, dialogProc );

	return m_iResult;
}

void getUserInput( HWND const hwndDlg, int id, int & iResult )
{
	if ( GetWindowText( GetDlgItem( hwndDlg, id ), m_wBuffer, BUFLEN ) )
	{
		int iValue;
		if ( swscanf_s( m_wBuffer, L"%d", &iValue ) > 0 )
			iResult = iValue;
	}
}

void enableGridTypeControls( HWND const hwndDlg, BOOL const bState )
{
	EnableWindow( GetDlgItem( hwndDlg, IDM_CHANGE_GRID_TYPE_WIDTH  ), bState );
	EnableWindow( GetDlgItem( hwndDlg, IDM_CHANGE_GRID_TYPE_HEIGHT ), bState );
	EnableWindow( GetDlgItem( hwndDlg, IDM_GRID_TYPE_HEX  ), bState );
	EnableWindow( GetDlgItem( hwndDlg, IDM_GRID_TYPE_RECT ), bState );
}

static INT_PTR CALLBACK dialogProc
( 
	HWND   const hDlg, 
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		m_iWidth    = GridDimensions::GridWidthVal();
		m_iHeight   = GridDimensions::GridHeightVal();
		m_iGridType = (GridDimensions::GetNrOfNeigbors() == 6) ? IDM_GRID_TYPE_HEX : IDM_GRID_TYPE_RECT;

		swprintf_s( m_wBuffer, BUFLEN, L"%d", m_iWidth );
		Util::SetText( GetDlgItem( hDlg, IDM_CHANGE_GRID_TYPE_WIDTH ), m_wBuffer );

		swprintf_s( m_wBuffer, BUFLEN, L"%d", m_iHeight );
		Util::SetText( GetDlgItem( hDlg, IDM_CHANGE_GRID_TYPE_HEIGHT ), m_wBuffer );

		SendDlgItemMessage( hDlg, m_iGridType, BM_CLICK, 0, 0 );
		SendDlgItemMessage( hDlg, IDM_RESET,   BM_CLICK, 0, 0 );

		return TRUE;
	}

	case WM_COMMAND:
	{
		int const wmId = LOWORD(wParam);
		switch (wmId)
		{

		case IDOK:
			getUserInput( hDlg, IDM_CHANGE_GRID_TYPE_WIDTH,  m_iWidth );
			getUserInput( hDlg, IDM_CHANGE_GRID_TYPE_HEIGHT, m_iHeight );
			EndDialog( hDlg, 0 );
			break;

		case IDCANCEL:
			m_iResult = -1;
			EndDialog( hDlg, 0 );
			break;

		case IDM_GRID_TYPE_RECT:
		case IDM_GRID_TYPE_HEX:
			m_iGridType = wmId;
			break;

		case IDM_RESET:
		case IDM_RESTART:
			enableGridTypeControls( hDlg, false );
			m_iResult = wmId;
			break;

		case IDM_CHANGE_GRID_TYPE:
			enableGridTypeControls( hDlg, true );
			m_iResult = wmId;
			break;

		default:
			break;
		}
	}
		break;

	default:
		break;
	}

	return FALSE;
}
