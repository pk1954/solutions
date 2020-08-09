// win32_NNetAppMenu.h
//
// NNetWindows

#pragma once

#include <string>
#include "ObserverInterface.h"

using std::wstring;

class WinManager;
class CommandStack;
class ComputeThread;
class NNetModelStorage;

class NNetAppMenu : public ObserverInterface
{
public:
	virtual ~NNetAppMenu() {}

	void Start
	( 
		HWND                     const, 
		ComputeThread    const * const, 
		WinManager       const * const,
		NNetModelStorage const * const,
		CommandStack     const * const,
		Sound            const * const 
	);
	void SetAppTitle( wstring const, bool const );
	void AdjustUndoRedo( );

	virtual void Notify( bool const );

private:

	HWND                     m_hwndApp        { nullptr };
	HMENU                    m_hMenu          { nullptr };
	ComputeThread    const * m_pComputeThread { nullptr };
	WinManager       const * m_pWinManager    { nullptr };
	NNetModelStorage const * m_pStorage       { nullptr };
	CommandStack     const * m_pCommandStack  { nullptr };
	Sound            const * m_pSound         { nullptr };
};