// AppTitle.ixx 
//
// NNetWindows

module;

#include <Windows.h>
#include <string>

export module NNetWin32:AppTitle;

import Win32_Util_Resource;
import ObserverInterface;
import Win32_Util;
import NNetModel;

using std::wstring;

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

	inline static wstring const PRODUCT_NAME 
	{ 
		L"NNetSimu 5.3 "
#ifndef NDEBUG
		L"release"
#else
		L"debug"
#endif
	};

private:
	void setAppTitle()
	{
		wstring wstr { m_bUnsavedChanges ? L" * " : L"" };
		if (m_pNMRI->ModelLocked())
			wstr += L"  +++ model locked +++";
		::SetApplicationTitle
		(
			m_hwndApp, 
			PRODUCT_NAME,
			m_pNMRI->GetModelFilePath() + wstr 
		);
	}

	HWND                             m_hwndApp         { nullptr };
	NNetModelReaderInterface const * m_pNMRI           { nullptr };
	bool                             m_bUnsavedChanges { false }; 
};
