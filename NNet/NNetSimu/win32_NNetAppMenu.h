// win32_NNetAppMenu.h
//
// NNetAppMenu

#pragma once

#include <string>

using std::wstring;

class WinManager;
class ComputeThread;
class NNetModelStorage;

class NNetAppMenu
{
public:
	virtual ~NNetAppMenu() {}

	void Start
	( 
		HWND                  const, 
		ComputeThread const * const, 
		WinManager    const * const
	);
	void AdjustVisibility( );
	void Stop( );

	static void SetAppTitle( wstring const, bool const );

private:
	void enableMenues( UINT const );

	inline static HWND m_hwndApp { nullptr };

	HMENU                 m_hMenu          { nullptr };
	ComputeThread const * m_pComputeThread { nullptr };
	WinManager    const * m_pWinManager    { nullptr };
};