// win32_NNetAppMenu.ixx
//
// NNetWindows

module;

#include <string>
#include <memory>
#include <Windows.h>

export module NNetWin32:NNetAppMenu;

import ObserverInterface;
import SoundInterface;
import WinManager;
import Win32_Util;
import OnOffPair;
import Commands;
import NNetModel;
import NNetPreferences;
import :ComputeThread;

using std::wstring;
using std::unique_ptr;

export class NNetAppMenu : public ObserverInterface
{
public:
	NNetAppMenu();
	~NNetAppMenu() final;

	void Start
	(
		HWND            const, 
		ComputeThread   const &, 
		CommandStack    const &,
		Sound           const &,
		NNetPreferences const &
	);
	void AdjustUndoRedo();
	void Notify(bool const) final;

private:

	HWND                    m_hwndApp        { nullptr };
	HMENU                   m_hMenu          { nullptr };
	HMENU                   m_hMenuSigGen    { nullptr };
	ComputeThread   const * m_pComputeThread { nullptr };
	NNetPreferences const * m_pPreferences   { nullptr };
	CommandStack    const * m_pCommandStack  { nullptr };
	Sound           const * m_pSound         { nullptr };

	unique_ptr<OnOffPair> m_upOnOffArrows;
	unique_ptr<OnOffPair> m_upOnOffSound;
	unique_ptr<OnOffPair> m_upOnOffAutoOpen;
	unique_ptr<OnOffPair> m_upOnOffSensorPoints;
	unique_ptr<OnOffPair> m_upOnOffPerfMonMode;
	unique_ptr<OnOffPair> m_upOnOffColorMenu;
};