// win32_NNetAppMenu.h
//
// NNetAppMenu

#pragma once

#include <string>
#include "win32_appMenu.h"

using std::wstring;

class NNetModelStorage;
class NNetWorkThreadInterface;

class NNetAppMenu : public AppMenu
{
public:
	virtual ~NNetAppMenu() {}

	virtual void Initialize
	( 
		HWND                            const, 
		NNetWorkThreadInterface const * const, 
		WinManager              const * const
	);
	virtual void AdjustVisibility( );
	virtual void Start( );
	virtual void Stop( );

	static void SetAppTitle( wstring const = L"" );

private:
	void enableMenues( UINT const );

	inline static HWND m_hwndApp { nullptr };

	HMENU                           m_hMenu                { nullptr };
	NNetWorkThreadInterface const * m_pWorkThreadInterface { nullptr };
	WinManager              const * m_pWinManager          { nullptr };
};