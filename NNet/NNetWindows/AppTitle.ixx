// AppTitle.ixx 
//
// NNetWindows

export module NNetWin32:AppTitle;

import std;
import WinBasics;
import Win32_Util_Resource;
import ObserverInterface;
import Win32_Util;
import NNetModel;

using std::wstring;

export class AppTitle : public ObserverInterface
{
public:
	wstring const LAST_EDIT { L"LastEdit" };

	void Initialize
	(
		HWND    const   hwndApp,
		wstring const & wstrProductName
	)
	{
		m_hwndApp          = hwndApp;
		m_pwstrProductName = &wstrProductName;
	}

	void SetModelInterface(NNetModelWriterInterface * const pNMWI)
	{
		m_pNMWI = pNMWI;
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
		wstring wstr { m_bUnsavedChanges ? L" * " : L"" };
		if (m_pNMWI->ModelLocked())
			wstr += L"  +++ model locked +++";
		::SetApplicationTitle
		(
			m_hwndApp, 
			*m_pwstrProductName,
			m_pNMWI->GetModelFilePath() + wstr 
		);
	}

	HWND                       m_hwndApp          { nullptr };
	wstring            const * m_pwstrProductName { nullptr };
	NNetModelWriterInterface * m_pNMWI            { nullptr };
	bool                       m_bUnsavedChanges  { false }; 
};
