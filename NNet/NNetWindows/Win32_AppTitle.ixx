// Win32_AppTitle.ixx 
//
// NNetWindows

module;

#include <Windows.h>

export module AppTitle;

import Win32_Util_Resource;
import ObserverInterface;
import Win32_Util;
import NNetModel;

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
			L"NNetSimu",
			m_pNMRI->GetModelFilePath() + (m_bUnsavedChanges ? L" * " : L"") 
		);
	}

	HWND                             m_hwndApp        { nullptr };
	NNetModelReaderInterface const * m_pNMRI          { nullptr };
	bool                             m_bUnsavedChanges{ false }; 
};
