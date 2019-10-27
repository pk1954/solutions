// win32_stdDialogBox.cpp :
//
// Win32_utilities

#include "stdafx.h"
#include <string>
#include <sstream>
#include <wchar.h>
#include <stdio.h>
#include "win32_util.h"
#include "win32_util_resource.h"

using std::wostringstream;
using std::wstring;

static float   m_fValue;     // the value to be edited in the dialog
static wstring m_wstrUnit;   // the text to be displayed right of the edit box
static wstring m_wstrTitle;  // the title bar text of the dialog

class StdDialogBox
{
private:
	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );

public:
	static float Show
	( 
		HWND    const hwndParent, 
		float         fValue,
		wstring const wstrTitle,
		wstring const wstrUnit
	)
	{
		m_wstrTitle = wstrTitle;
		m_wstrUnit  = wstrUnit;
		m_fValue    = fValue;

		if ( IDOK == DialogBox( nullptr, MAKEINTRESOURCE(IDD_STD_EDIT_DIALOG), hwndParent, dialogProc ) )
			fValue = m_fValue;

		return fValue;
	}

private:
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
			wostringstream m_wstrBuffer;
			m_wstrBuffer << m_fValue;
			SetWindowText( hDlg, m_wstrTitle.c_str() );
			SetWindowText( GetDlgItem( hDlg, IDD_EDIT_CTL ), m_wstrBuffer.str().c_str() );
			SetWindowText( GetDlgItem( hDlg, IDC_STATIC ), m_wstrUnit.c_str() );
			SendMessage( hDlg, DM_SETDEFID, IDOK, 0);
			SendMessage( GetDlgItem( hDlg, IDCANCEL ), BM_SETSTYLE, BS_PUSHBUTTON, 0);
			return TRUE;
		}

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				static int const BUFLEN = 20;
				static wchar_t wBuffer[BUFLEN];

				if ( GetWindowText( GetDlgItem( hDlg, IDD_EDIT_CTL ), wBuffer, BUFLEN ) )
				{
					wstring wstrEdit( wBuffer );
					try
					{
						m_fValue = stof( wstrEdit );
						EndDialog( hDlg, LOWORD(wParam) );
					} catch(...)
					{
						MessageBeep( MB_ICONWARNING );
					}
				}

				return TRUE;
			}
			break;

		default:
			break;
		}

		return FALSE;
	}
};
