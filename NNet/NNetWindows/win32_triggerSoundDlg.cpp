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
#include "win32_triggerSoundDlg.h"

using std::wostringstream;
using std::wstring;

void TriggerSoundDialog::initEditField(HWND const hDlg, int const id, unsigned long const ulValue)
{
	wostringstream m_wstrBuffer;
	m_wstrBuffer << ulValue;
	SetWindowText(GetDlgItem(hDlg, id), m_wstrBuffer.str().c_str());
}

unsigned long TriggerSoundDialog::evaluateEditField(HWND const hDlg, int const id)
{
	static int const BUFLEN { 20 };

	wchar_t wBuffer[BUFLEN];

	if (GetWindowText(GetDlgItem(hDlg, id), wBuffer, BUFLEN))
	{
		wstring wstrEdit(wBuffer);
		return stoul(wstrEdit);
	}

	return 0;
}

void TriggerSoundDialog::handleOnOff(HWND const hDlg)
{
	bool bTriggerSoundOn { IsDlgButtonChecked(hDlg, IDC_TRIGGER_SOUND_ON) == BST_CHECKED };
	EnableWindow(GetDlgItem(hDlg, IDC_TRIGGER_SOUND_FREQ), bTriggerSoundOn); 
	EnableWindow(GetDlgItem(hDlg, IDC_TRIGGER_SOUND_MSEC), bTriggerSoundOn); 
	EnableWindow(GetDlgItem(hDlg, IDC_TRIGGER_SOUND_TEST), bTriggerSoundOn);
}

void TriggerSoundDialog::evaluate(HWND const hDlg)
{
	m_soundDesc.m_bOn       = IsDlgButtonChecked(hDlg, IDC_TRIGGER_SOUND_ON) == BST_CHECKED;
	m_soundDesc.m_frequency = m_soundDesc.m_bOn ? Hertz    (evaluateEditField(hDlg, IDC_TRIGGER_SOUND_FREQ)) : 0_Hertz;
	m_soundDesc.m_duration  = m_soundDesc.m_bOn ? MilliSecs(evaluateEditField(hDlg, IDC_TRIGGER_SOUND_MSEC)) : 0_MilliSecs;
}

void TriggerSoundDialog::onCommand(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	WORD id { LOWORD(wParam) };
	switch (id)
	{
	case IDOK:
		evaluate(hDlg);
		EndDialog(hDlg, INT_PTR(true));
		break;

	case IDCANCEL:
		EndDialog(hDlg, INT_PTR(false));
		break;

	case IDC_TRIGGER_SOUND_ON:
		handleOnOff(hDlg);
		break;

	case IDC_TRIGGER_SOUND_TEST:
		evaluate(hDlg);
		m_pSound->Beep(m_soundDesc);
		break;

	default:
		break;
	}
}	

static INT_PTR CALLBACK dialogProc
(
	HWND   const hDlg, 
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam
)
{
	TriggerSoundDialog * pDlg = reinterpret_cast<TriggerSoundDialog *>(GetWindowLongPtr(hDlg, DWLP_USER));
	switch (message)
	{
	case WM_INITDIALOG:
		pDlg = reinterpret_cast<TriggerSoundDialog *>(lParam);
		pDlg->initEditField(hDlg, IDC_TRIGGER_SOUND_FREQ, pDlg->m_soundDesc.m_frequency.GetValue());
		pDlg->initEditField(hDlg, IDC_TRIGGER_SOUND_MSEC, pDlg->m_soundDesc.m_duration .GetValue());
		CheckDlgButton     (hDlg, IDC_TRIGGER_SOUND_ON,   pDlg->m_soundDesc.m_bOn ? BST_CHECKED : BST_UNCHECKED);
		pDlg->handleOnOff(hDlg);
		::SetWindowLongPtr(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pDlg));
		return INT_PTR(true);

	case WM_COMMAND:
		pDlg->onCommand(hDlg, wParam, lParam);
		break;

	default:
		break;
	}

	return INT_PTR(false);
}

void TriggerSoundDialog::Show(HWND const hwndParent)
{
	DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_TRIGGER_SOUND_DLG), hwndParent, dialogProc, reinterpret_cast<LPARAM>(this));
}
