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
using std::bit_cast;
using std::wstring;

TriggerSoundDialog::TriggerSoundDialog
(
	Sound      const * const pSound, 
	SoundDescr const &       soundDesc 
)
  : m_pSound   (pSound),
	m_soundDesc(soundDesc)
{};

unsigned long TriggerSoundDialog::evaluateEditField
(
	HWND const hDlg, 
	int  const id
) const
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

void TriggerSoundDialog::handleOnOff(HWND const hDlg) const
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

void TriggerSoundDialog::OnInitDlg(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	Util::SetEditField(GetDlgItem(hDlg, IDC_TRIGGER_SOUND_FREQ), m_soundDesc.m_frequency.GetValue());
	Util::SetEditField(GetDlgItem(hDlg, IDC_TRIGGER_SOUND_MSEC), m_soundDesc.m_duration .GetValue());
	CheckDlgButton               (hDlg, IDC_TRIGGER_SOUND_ON,    m_soundDesc.m_bOn ? BST_CHECKED : BST_UNCHECKED);
	handleOnOff(hDlg);
}

bool TriggerSoundDialog::OnOK(HWND const hDlg)
{
	evaluate(hDlg);
	return true;
}

void TriggerSoundDialog::OnCommand(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	WORD id { LOWORD(wParam) };
	switch (id)
	{
	case IDC_TRIGGER_SOUND_ON:
		handleOnOff(hDlg);
		break;

	case IDC_TRIGGER_SOUND_TEST:
		evaluate(hDlg);
		m_pSound->Beep(m_soundDesc);
		break;

	default:
		StdDialogBox::OnCommand(hDlg,wParam, lParam);
		break;
	}
}	
