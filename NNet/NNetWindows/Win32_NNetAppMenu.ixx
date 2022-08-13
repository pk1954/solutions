// win32_NNetAppMenu.ixx
//
// NNetWindows

module;

#include <string>
#include <Windows.h>
#include "win32_util.h"
#include "win32_MainWindow.h"
#include "CommandStack.h"
#include "NNetModelReaderInterface.h"

export module NNetAppMenu;

import ObserverInterface;
import SoundInterface;
import WinManager;
import ComputeThread;

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
		MainWindow    const &
	);
	void AdjustUndoRedo();
	void Notify(bool const) final;

	void SetModelInterface(NNetModelReaderInterface const * p) 
	{ 
		m_pNMRI = p;
	}

private:

	class OnOffPair;

	void enable(unsigned int const, bool const);
	void addSigGenMenuEntries();
	void delSigGenMenuEntries();

	HWND                             m_hwndApp        { nullptr };
	HMENU                            m_hMenu          { nullptr };
	HMENU                            m_hMenuSigGen    { nullptr };
	ComputeThread            const * m_pComputeThread { nullptr };
	MainWindow               const * m_pMainWindow    { nullptr };
	WinManager               const * m_pWinManager    { nullptr };
	CommandStack             const * m_pCommandStack  { nullptr };
	Sound                    const * m_pSound         { nullptr };
	NNetModelReaderInterface const * m_pNMRI          { nullptr };

	unique_ptr<OnOffPair> m_upOnOffArrows;
	unique_ptr<OnOffPair> m_upOnOffSound;
	unique_ptr<OnOffPair> m_upOnOffAutoOpen;
	unique_ptr<OnOffPair> m_upOnOffSensorPoints;
	unique_ptr<OnOffPair> m_upOnOffPerfMonMode;
};