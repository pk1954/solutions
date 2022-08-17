// Win32_AppTitle.ixx 
//
// NNetWindows

module;

#include "win32_util_resource.h"
#include "NNetModelReaderInterface.h"

export module AppTitle;

import ObserverInterface;
import Win32_Util;

export class AppTitle : public ObserverInterface
{
public:
	void Initialize(HWND const hwndApp)
	{
		m_hwndApp = hwndApp;
	}

	void SetModelInterface(NNetModelReaderInterface const * const pNMRI)
	{
		m_pNMRI = pNMRI;
	}

	void Notify(bool const bImmediate) final 
	{ 
		SetUnsavedChanges(true);
	}

	void SetUnsavedChanges(bool const bState)
	{
		m_bUnsavedChanges = bState;
		setAppTitle();
	}

	bool AnyUnsavedChanges() const 
	{ 
		return m_bUnsavedChanges; 
	}

private:
	void setAppTitle()
	{
		Util::SetApplicationTitle
		(
			m_hwndApp, 
			IDS_APP_TITLE, 
			m_pNMRI->GetModelFilePath() + (m_bUnsavedChanges ? L" * " : L"") 
		);
	}

	HWND                             m_hwndApp        { nullptr };
	NNetModelReaderInterface const * m_pNMRI          { nullptr };
	bool                             m_bUnsavedChanges{ false }; 
};
