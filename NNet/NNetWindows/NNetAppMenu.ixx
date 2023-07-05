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
import Commands;
import NNetModel;
import :ComputeThread;
import :Preferences;

using std::wstring;
using std::unique_ptr;

export class NNetAppMenu : public ObserverInterface
{
public:
	NNetAppMenu();
	~NNetAppMenu() final;

	void Start
	(
		HWND          const, 
		ComputeThread const &, 
		WinManager    const &,
		CommandStack  const &,
		Sound         const &,
		Preferences   const &
	);
	void AdjustUndoRedo();
	void Notify(bool const) final;

private:

	class OnOffPair;

	void enable(unsigned int const, bool const);

	HWND                             m_hwndApp        { nullptr };
	HMENU                            m_hMenu          { nullptr };
	HMENU                            m_hMenuSigGen    { nullptr };
	ComputeThread            const * m_pComputeThread { nullptr };
	Preferences              const * m_pPreferences   { nullptr };
	WinManager               const * m_pWinManager    { nullptr };
	CommandStack             const * m_pCommandStack  { nullptr };
	Sound                    const * m_pSound         { nullptr };

	unique_ptr<OnOffPair> m_upOnOffScales;
	unique_ptr<OnOffPair> m_upOnOffArrows;
	unique_ptr<OnOffPair> m_upOnOffSound;
	unique_ptr<OnOffPair> m_upOnOffAutoOpen;
	unique_ptr<OnOffPair> m_upOnOffSensorPoints;
	unique_ptr<OnOffPair> m_upOnOffPerfMonMode;
	unique_ptr<OnOffPair> m_upOnOffColorMenu;
};