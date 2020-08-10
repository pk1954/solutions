// UnsavedChangesObserver.h
//
// NNetWindows

#pragma once

#include "ObserverInterface.h"
#include "Resource.h"
#include "win32_util.h"
#include "NNetModelStorage.h"

class UnsavedChangesObserver : public ObserverInterface
{
public:
	virtual ~UnsavedChangesObserver() {}

	inline void Start
	( 
		HWND                     const hwndApp,
		NNetModelStorage const * const pStorage
	)
	{
		m_hwndApp  = hwndApp;
		m_pStorage = pStorage;

		setAppTitle( L"", false );
	}

	virtual void Notify( bool const bImmediatelly )
	{
		setAppTitle
		( 
			m_pStorage->GetModelPath(), 
			m_pStorage->UnsavedChanges()
		);
	}

private:

	HWND                     m_hwndApp  { nullptr };
	NNetModelStorage const * m_pStorage { nullptr };

	void setAppTitle( wstring const wstrAdd, bool const bUnsavedChanges )
	{
		Util::SetApplicationTitle( m_hwndApp, IDS_APP_TITLE, wstrAdd + (bUnsavedChanges ? L" * " : L"") );
	}
};