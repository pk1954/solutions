// win32_NNetEditor.cpp 
//
// NNetWindows

#include "stdafx.h"
#include "commctrl.h"
#include "Resource.h"
#include "win32_util.h"
#include "win32_tooltip.h"
#include "win32_NNetReadBuffer.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetEditor.h"

NNetEditorWindow::NNetEditorWindow( )
	: BaseDialog( ),
	m_pReadBuffer         ( nullptr ),
	m_pWorkThreadInterface( nullptr )
{ }

void NNetEditorWindow::Start
(  
	HWND                      const hwndParent,
	NNetWorkThreadInterface * const pWorkThreadInterface,
	NNetReadBuffer          * const pReadBuffer
)
{
	m_pWorkThreadInterface = pWorkThreadInterface;
	m_pReadBuffer          = pReadBuffer;

	StartBaseDialog( hwndParent, MAKEINTRESOURCE( IDD_EDITOR ), nullptr ); 

	UpdateEditControls( );

	//CreateWindowToolTip( L"The editor allows to manipulate the model manually by using the left mouse button." );
	//CreateBalloonToolTip( IDM_MOVE, L"Left mouse button moves the model on the screen (no changes to the model). " );

	m_pReadBuffer->RegisterObserver( this );
}

void NNetEditorWindow::Stop( )
{
	DestroyWindow( );
	m_pWorkThreadInterface = nullptr;
	m_pReadBuffer          = nullptr;
}

NNetEditorWindow::~NNetEditorWindow( )
{
	m_pWorkThreadInterface = nullptr;
	m_pReadBuffer          = nullptr;
}

LRESULT NNetEditorWindow::SendClick( int const item ) const
{
	HWND    const hwndOld { SetActiveWindow( GetWindowHandle( ) ) };
	LRESULT const res     { SendDlgItemMessage( item, BM_CLICK, 0, 0 ) };
	(void)SetActiveWindow( hwndOld );
	return res;
}

void NNetEditorWindow::UpdateEditControls( ) // Set state of all window widgets according to mode (edit/simu)
{
//	EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );
//	m_pReadBuffer->ReleaseReadBuffer( );
}

LRESULT NNetEditorWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
	switch (message)
	{

	case WM_PAINT:
		UpdateEditControls( );
		break;

	case WM_COMMAND:
	{
		WORD const wId { LOWORD( wParam ) };

		switch ( wId )
		{
		case IDM_MOVE:
			break;

		case IDM_EDIT_UNDO:
		case IDM_EDIT_REDO:
			PostCommand2Application( wId, 0 );
			break;

		default:
			assert( false );
			break;
		}
	}
	break;

	case WM_ACTIVATE:
		PostCommand2Application( IDD_EDITOR, TRUE );
		break;

	case WM_CLOSE:
		AnimateWindow( GetWindowHandle(), 200, AW_HIDE | AW_VER_POSITIVE );
		PostCommand2Application( IDD_EDITOR, FALSE );
		return TRUE; 

	default:
		break;
	}

	return FALSE;
}
