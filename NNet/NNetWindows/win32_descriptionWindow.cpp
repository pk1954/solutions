// win32_descriptionWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Richedit.h"
#include "Resource.h"
#include "win32_descriptionWindow.h"

static WORD const ID_EDIT_CTRL { 42 };

void DescriptionWindow::Start( HWND const hwndParent )
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

    m_hwndEdit = CreateWindowEx
    ( 
        0,
        L"EDIT",                 // predefined class 
        NULL,        
        WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL, 
        0, 0, 0, 0,              // set size in WM_SIZE message 
        hwndDlg,                 // parent window 
        (HMENU)ID_EDIT_CTRL,     // control id
        GetModuleHandle( nullptr ), 
        NULL
    );          
    //m_hwndEdit = CreateWindow
    //( 
    //    L"EDIT",                 // predefined class 
    //    NULL,        
    //    WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL, 
    //    0, 0, 0, 0,              // set size in WM_SIZE message 
    //    hwndDlg,                 // parent window 
    //    (HMENU)ID_EDIT_CTRL,     // control id
    //    GetModuleHandle( nullptr ), 
    //    NULL
    //);          
}

void DescriptionWindow::Stop()
{
    DestroyWindow();
}

void DescriptionWindow::ClearDescription()
{
    Edit_SetText( m_hwndEdit, L"" );
}

void DescriptionWindow::SetDescription( wstring const wstrDesc )
{
    Edit_SetText( m_hwndEdit, wstrDesc.c_str() );
}

int const DescriptionWindow::GetLineCount() const
{
    return Edit_GetLineCount( m_hwndEdit );
}

bool const DescriptionWindow::GetDescriptionLine( int const iLineNr, wstring & wstrDst ) const
{
    if ( iLineNr < GetLineCount() )
    {
        static const int BUFLEN { 1024 };
        alignas(int) wchar_t buffer[BUFLEN];  // Edit_GetLine interpretes begin of buffer as int
        int iLineIndex  = Edit_LineIndex ( m_hwndEdit, iLineNr );
        int iLineLength = Edit_LineLength( m_hwndEdit, iLineIndex );
        int iCharsRead  = Edit_GetLine   ( m_hwndEdit, iLineNr, buffer, BUFLEN );
        for ( int i = 0; i < iCharsRead; ++i )  // copy line to wstrDst 
        {                                       // removing CR and LF characters
            wchar_t c { buffer[i] };
            if ( (c != L'\r') &&(c != L'\n') )
                wstrDst += c;
         }
            
        return true;
    }
    return false;
}

bool DescriptionWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
    // control notifications

    if ( (LOWORD(wParam) == ID_EDIT_CTRL) && (HIWORD(wParam) == EN_CHANGE) )
    {
        m_bDirty = true;
        return true; 
    }

    // other commands

    int const wmId = LOWORD( wParam );

    switch (wmId) 
    { 
    case IDM_SELECT_ALL:
        Edit_SetSel( m_hwndEdit, 0, -1 ); 
        return true; 

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
        return true; 

    default:
        break;
    }

    return BaseWindow::OnCommand( wParam, lParam, pixPoint );
}

bool DescriptionWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
    MoveWindow
    (
        m_hwndEdit, 
        0, 0,           // starting x- and y-coordinates 
        LOWORD(lParam), // width of client area 
        HIWORD(lParam), // height of client area 
        TRUE            // repaint window 
    );

    return true;
}
