// win32_triggerSoundDlg.cpp :
//
// NNetWindows

#include "stdafx.h"
#include <string>
#include <sstream>
#include <wchar.h>
#include <stdio.h>
#include "Resource.h"
#include "win32_util.h"
#include "win32_sound.h"
#include "win32_util_resource.h"
#include "win32_triggerSoundDlg.h"

using std::wostringstream;
using std::wstring;

bool TriggerSoundDialog::IsTriggerSoundActive( )
{
	return m_bSoundActive;
}

Hertz TriggerSoundDialog::GetFrequency()
{
	return m_frequency;
}

MilliSecs TriggerSoundDialog::GetDuration()
{
	return m_duration;
}

void TriggerSoundDialog::initEditField( HWND const hDlg, int const id, unsigned long const ulValue )
{
	wostringstream m_wstrBuffer;
	m_wstrBuffer << ulValue;
	SetWindowText( GetDlgItem( hDlg, id ), m_wstrBuffer.str().c_str() );
}

unsigned long TriggerSoundDialog::evaluateEditField( HWND const hDlg, int const id )
{
	static int const BUFLEN { 20 };

	wchar_t wBuffer[BUFLEN];

	if ( GetWindowText( GetDlgItem( hDlg, id ), wBuffer, BUFLEN ) )
	{
		wstring wstrEdit( wBuffer );
		return stoul( wstrEdit );
	}

	return 0;
}

void TriggerSoundDialog::handleOnOff( HWND const hDlg )
{
	bool bTriggerSoundOn { IsDlgButtonChecked( hDlg, IDC_TRIGGER_SOUND_ON ) == BST_CHECKED };
	EnableWindow( GetDlgItem( hDlg, IDC_TRIGGER_SOUND_FREQ ), bTriggerSoundOn ); 
	EnableWindow( GetDlgItem( hDlg, IDC_TRIGGER_SOUND_MSEC ), bTriggerSoundOn ); 
	EnableWindow( GetDlgItem( hDlg, IDC_TRIGGER_SOUND_TEST ), bTriggerSoundOn );
}

static INT_PTR CALLBACK dialogProc
( 
	HWND   const hDlg, 
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam
)
{
	TriggerSoundDialog * pDlg = reinterpret_cast<TriggerSoundDialog *>(GetWindowLongPtr( hDlg, DWLP_USER ));
	switch (message)
	{
	case WM_INITDIALOG:
		pDlg = reinterpret_cast<TriggerSoundDialog *>(lParam);
		pDlg->initEditField( hDlg, IDC_TRIGGER_SOUND_FREQ, pDlg->m_frequency.GetValue() );
		pDlg->initEditField( hDlg, IDC_TRIGGER_SOUND_MSEC, pDlg->m_duration .GetValue() );
		CheckDlgButton     ( hDlg, IDC_TRIGGER_SOUND_ON,   pDlg->m_bSoundActive ? BST_CHECKED : BST_UNCHECKED );
		pDlg->handleOnOff( hDlg );
		::SetWindowLongPtr( hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pDlg) );
		return TRUE;

	case WM_COMMAND:
		{
			WORD id { LOWORD(wParam) };
			switch ( id )
			{
			case IDOK:
				pDlg->m_bSoundActive = IsDlgButtonChecked( hDlg, IDC_TRIGGER_SOUND_ON ) == BST_CHECKED;
				pDlg->m_frequency = Hertz    ( pDlg->evaluateEditField( hDlg, IDC_TRIGGER_SOUND_FREQ ) );
				pDlg->m_duration  = MilliSecs( pDlg->evaluateEditField( hDlg, IDC_TRIGGER_SOUND_MSEC ) );
				EndDialog( hDlg, true );
				return TRUE;

			case IDCANCEL:
				EndDialog( hDlg, false );
				return TRUE;

			case IDC_TRIGGER_SOUND_ON:
				pDlg->handleOnOff( hDlg );
				break;

			case IDC_TRIGGER_SOUND_TEST:
				Sound::Beep
				( 
					Hertz    ( pDlg->evaluateEditField( hDlg, IDC_TRIGGER_SOUND_FREQ ) ),
				    MilliSecs( pDlg->evaluateEditField( hDlg, IDC_TRIGGER_SOUND_MSEC ) )
				);
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

void TriggerSoundDialog::Show( HWND const hwndParent )
{
	DialogBoxParam( nullptr, MAKEINTRESOURCE(IDD_TRIGGER_SOUND_DLG), hwndParent, dialogProc, reinterpret_cast<LPARAM>(this) );
}
