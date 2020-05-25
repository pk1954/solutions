// win32_NNetAppMenu.h
//
// NNetAppMenu

#pragma once

#include <string>

using std::wstring;

class WinManager;
class ComputeThread;
class NNetModelStorage;
class WorkThreadInterface;

class NNetAppMenu
{
public:
	virtual ~NNetAppMenu() {}

	void Initialize
	( 
		HWND                        const, 
		ComputeThread       const * const, 
		WorkThreadInterface const * const, 
		WinManager          const * const
	);
	void AdjustVisibility( );
	void Start( );
	void Stop( );

	static void SetAppTitle( wstring const, bool const );

private:
	void enableMenues( UINT const );

	inline static HWND m_hwndApp { nullptr };

	HMENU                       m_hMenu                { nullptr };
	ComputeThread       const * m_pComputeThread       { nullptr };
	WorkThreadInterface const * m_pWorkThreadInterface { nullptr };
	WinManager          const * m_pWinManager          { nullptr };
};