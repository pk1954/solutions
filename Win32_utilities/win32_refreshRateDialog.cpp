// win32_refreshRateDialog.cpp :
//

#include "stdafx.h"
#include <wchar.h>
#include <stdio.h>
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_refreshRateDialog.h"

using namespace std::chrono;

static int const    BUFLEN = 20;
static wchar_t      m_wBuffer[BUFLEN];
static milliseconds m_msRefreshRate;

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
		swprintf_s( m_wBuffer, BUFLEN, L"%lld", m_msRefreshRate.count() );
		Util::SetText( GetDlgItem( hDlg, IDD_REFRESH_RATE_EDIT_CTL ), m_wBuffer	);
        return TRUE;
	}

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
			if ( GetWindowText( GetDlgItem( hDlg, IDD_REFRESH_RATE_EDIT_CTL ), m_wBuffer, BUFLEN ) )
			{
				int iRate;
				if ( swscanf_s( m_wBuffer, L"%d", &iRate ) > 0 )
				{
					m_msRefreshRate = static_cast<milliseconds>(iRate);
					EndDialog( hDlg, LOWORD(wParam) );
				}
				else
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

milliseconds RefreshRateDialog::Show( HWND const hwndParent, milliseconds msRefreshRate )
{
	m_msRefreshRate = msRefreshRate;

    int iRes = DialogBox( nullptr, MAKEINTRESOURCE(IDM_REFRESH_RATE_DIALOG), hwndParent, dialogProc );

	DWORD dw = GetLastError();

	return m_msRefreshRate;
}
