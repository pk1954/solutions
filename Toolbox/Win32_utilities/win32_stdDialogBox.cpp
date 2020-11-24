// win32_stdDialogBox.cpp
//
// Win32_utilities

#include "stdafx.h"
#include <sstream>
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_stdDialogBox.h"

using std::wostringstream;

static float   m_fValue;     // the value to be edited in the dialog
static wstring m_wstrUnit;   // the text to be displayed right of the edit box
static wstring m_wstrTitle;  // the title bar text of the dialog

float StdDialogBox::Show
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

void StdDialogBox::SetParameterValue( HWND const hwndEditField, float const fValue )
{
	wostringstream m_wstrBuffer;
	m_wstrBuffer << fValue;
	SetWindowText( hwndEditField, m_wstrBuffer.str().c_str() );
}

bool StdDialogBox::Evaluate( HWND const hwndEditField, float & fValue )
{
	static int const BUFLEN = 20;
	static wchar_t wBuffer[BUFLEN];

	float fNewValue { fValue };
	bool  bResult   { false };

	if ( GetWindowText( hwndEditField, wBuffer, BUFLEN ) )
	{
		wstring wstrEdit( wBuffer );

		for ( auto & c : wstrEdit )  // change german decimal comma to
			if ( c == L',' )         // decimal point
				c = L'.';
		try
		{
			fNewValue = stof( wstrEdit );
			fValue = fNewValue;
			bResult = true;
		} catch(...)
		{
			MessageBeep( MB_ICONWARNING );
		}
	}

	SetParameterValue( hwndEditField, fValue );
	return bResult;
}

INT_PTR CALLBACK dialogProc
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
		SetWindowText( hDlg, m_wstrTitle.c_str() );
		StdDialogBox::SetParameterValue( GetDlgItem( hDlg, IDD_EDIT_CTL ), m_fValue );
		SetWindowText( GetDlgItem( hDlg, IDC_STATIC ), m_wstrUnit.c_str() );
		SendMessage( hDlg, DM_SETDEFID, IDOK, 0);
		SendMessage( GetDlgItem( hDlg, IDCANCEL ), BM_SETSTYLE, BS_PUSHBUTTON, 0);
		return INT_PTR(true);
	}

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK )
		{
			HWND hwndEditCtl { GetDlgItem( hDlg, IDD_EDIT_CTL ) };
			if ( StdDialogBox::Evaluate( hwndEditCtl, m_fValue ) )
				EndDialog( hDlg, LOWORD(wParam) );
			else 
				SetFocus( hwndEditCtl );
			return INT_PTR(true);
		}
		else if ( LOWORD(wParam) == IDCANCEL )
		{
			EndDialog( hDlg, LOWORD(wParam) );
			return INT_PTR(true);
		}
		break;

	default:
		break;
	}

	return INT_PTR(false);
}

