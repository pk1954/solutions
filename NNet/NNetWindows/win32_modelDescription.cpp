// win32_modelDescription.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Richedit.h"
#include "Resource.h"
#include "win32_messagePump.h"
#include "win32_modelDescription.h"

static HMENU const ID_EDIT_CTRL { (HMENU)1 };

void DescriptionWindow::Start
( 
    HWND const hwndParent,
    MessagePump & pump
)
{
    PixelRect rect( 100_PIXEL, 100_PIXEL, 200_PIXEL, 200_PIXEL );
    HWND const hwndDlg = StartBaseWindow
    ( 
        hwndParent, 
        CS_OWNDC | CS_DBLCLKS,
        L"ModelDescWindow",
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
        & rect,
        nullptr
    );

    SetWindowText( hwndDlg, L"Model description" );

    m_hwndEdit = CreateWindow
    ( 
        L"EDIT",                 // predefined class 
        NULL,        
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | 
        ES_MULTILINE |  ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 
        0, 0, 0, 0,              // set size in WM_SIZE message 
        hwndDlg,                 // parent window 
        ID_EDIT_CTRL,
        GetModuleHandle( nullptr ), 
        NULL
    );          

    pump.RegisterWindow( hwndDlg, true );
}

void DescriptionWindow::Stop( )
{
    DestroyWindow( );
}

void DescriptionWindow::ClearDescription( )
{
    Edit_SetText( m_hwndEdit, L"" );
}

void DescriptionWindow::SetDescription( wstring const wstrDesc )
{
    Edit_SetText( m_hwndEdit, wstrDesc.c_str() );
}

bool DescriptionWindow::GetDescriptionLine( int const iLineNr, wstring & wstrDst ) const
{
    static const int BUFLEN { 1024 };
    wchar_t buffer[BUFLEN];
    int iLineCount  = Edit_GetLineCount( m_hwndEdit );
    if ( iLineNr >= iLineCount )
        return false;
    int iLineIndex  = Edit_LineIndex ( m_hwndEdit, iLineNr );
    int iLineLength = Edit_LineLength( m_hwndEdit, iLineIndex );
    int iCharsRead  = Edit_GetLine   ( m_hwndEdit, iLineNr, buffer, BUFLEN );
    wstrDst.assign( buffer, iCharsRead );
    return true;
}

bool DescriptionWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
    int const wmId = LOWORD( wParam );

    switch (wmId) 
    { 
    case IDM_SELECT_ALL:
        Edit_SetSel( m_hwndEdit, 0, -1 ); 
        break; 

     case IDM_DELETE: 
     {
         DWORD dwSelStart { 0L };
         DWORD dwSelEnd   { 0L };
         ::SendMessage( m_hwndEdit, EM_GETSEL, (WPARAM)&dwSelStart, (LPARAM)&dwSelEnd );
         if ( dwSelStart == dwSelEnd )
         {
             int iNrOfChars { ::Edit_GetTextLength( m_hwndEdit ) };  
             if ( dwSelStart == iNrOfChars )                         // if cursor is at end
                 break;                                              // nothing to delete
             Edit_SetSel( m_hwndEdit, dwSelStart, dwSelStart + 1 ); 
         }
         ::SendMessage( m_hwndEdit, WM_CLEAR, 0, 0 ); 
     }
        break; 

    default:
        return false;
    }

    return true;
}

LRESULT DescriptionWindow::UserProc( UINT const uMsg, WPARAM const wParam, LPARAM const lParam )
{
    switch ( uMsg )
    {
    case WM_SIZE: 
        MoveWindow
        (
            m_hwndEdit, 
            0, 0,           // starting x- and y-coordinates 
            LOWORD(lParam), // width of client area 
            HIWORD(lParam), // height of client area 
            TRUE            // repaint window 
        );
        return 0; 

    case WM_CLOSE:    // Do not destroy, just hide  
        Show( false );
        return true;

    default: 
        break; 
    } 

    return DefWindowProc( uMsg, wParam, lParam );
}
