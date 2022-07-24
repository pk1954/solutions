// win32_NNetAppMenu.h
//
// NNetWindows

#pragma once

#include <string>
#include "win32_util.h"

import ObserverInterface;

using std::wstring;
using std::unique_ptr;

class MainWindow;
class WinManager;
class CommandStack;
class ComputeThread;
class NNetModelReaderInterface;

class NNetAppMenu : public ObserverInterface
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