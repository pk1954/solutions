// win32_modelDescription.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Richedit.h"
#include "Resource.h"
#include "win32_modelDescription.h"

static HMENU const ID_EDIT_CTRL { (HMENU)1 };

void DescriptionWindow::Start
( 
    HWND const hwndParent
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

//void DescriptionWindow::AppendDescriptionLine( wstring const wstrDesc )
//{
//    wstring const wstrLine { wstrDesc + L"\n\r" };
//    int iActLength = ::Edit_GetTextLength( m_hwndEdit );
//    Edit_SetSel    ( m_hwndEdit, iActLength, iActLength );
//    Edit_ReplaceSel( m_hwndEdit, reinterpret_cast<LPARAM>(wstrLine.c_str()) );
//}
//
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

LRESULT DescriptionWindow::UserProc( UINT const uMsg, WPARAM const wParam, LPARAM const lParam )
{
    switch ( uMsg )
    {
    case WM_COMMAND: 
        switch (wParam) 
        { 
        //case IDM_EDUNDO: 

        //     Send WM_UNDO only if there is something 
        //     to be undone. 

        //    if (SendMessage(hwndEdit, EM_CANUNDO, 0, 0)) 
        //        SendMessage(hwndEdit, WM_UNDO, 0, 0); 
        //    else 
        //    {
        //        MessageBox(hwndEdit, 
        //            "Nothing to undo.", 
        //            "Undo notification", MB_OK); 
        //    }
        //    break; 

        //case IDM_EDCUT: 
        //    SendMessage(hwndEdit, WM_CUT, 0, 0); 
        //    break; 

        //case IDM_EDCOPY: 
        //    SendMessage(hwndEdit, WM_COPY, 0, 0); 
        //    break; 

        //case IDM_EDPASTE: 
        //    SendMessage(hwndEdit, WM_PASTE, 0, 0); 
        //    break; 

        //case IDM_EDDEL: 
        //    SendMessage(hwndEdit, WM_CLEAR, 0, 0); 
        //    break; 

        //case IDM_WRAP: 
        //    SendMessage(hwndEdit, 
        //        EM_SETWORDBREAKPROC, 
        //        (WPARAM) 0, 
        //        (LPARAM) (EDITWORDBREAKPROC) WordBreakProc); 
        //    SendMessage(hwndEdit, 
        //        EM_FMTLINES, 
        //        (WPARAM) TRUE, 
        //        (LPARAM) 0); 
        //    SendMessage(hwndEdit, 
        //        EM_SETSEL, 
        //        0, -1); // select all text 
        //    SendMessage(hwndEdit, WM_CUT, 0, 0); 
        //    SendMessage(hwndEdit, WM_PASTE, 0, 0); 
        //    break; 

        default: 
            break;
        } 
        break; 

    case WM_SETFOCUS: 
        ::SetFocus( m_hwndEdit ); 
        return 0; 

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

    default: 
        break; 
    } 

    return DefWindowProc( uMsg, wParam, lParam );
}
