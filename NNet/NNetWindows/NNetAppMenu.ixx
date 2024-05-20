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
import CommandStack;
import NNetModel;
import :Compute;

using std::wstring;
using std::unique_ptr;

export class NNetAppMenu : public ObserverInterface
{
public:
	NNetAppMenu();
	~NNetAppMenu() final;

	void Start
	(
		HWND         const, 
		Compute      const &, 
		CommandStack const &
	);
	void SetModelInterface(NNetModelReaderInterface const * const);
	void AdjustUndoRedo();
	void Notify(bool const) final;

private:

	HWND                             m_hwndApp       { nullptr };
	HMENU                            m_hMenu         { nullptr };
	HMENU                            m_hMenuSigGen   { nullptr };
	Compute                  const * m_pCompute      { nullptr };
	CommandStack             const * m_pCommandStack { nullptr };
	NNetModelReaderInterface const * m_pNMRI         { nullptr };

	unique_ptr<OnOffPair> m_upOnOffArrows;
	unique_ptr<OnOffPair> m_upOnOffSound;
	unique_ptr<OnOffPair> m_upOnOffAutoOpen;
	unique_ptr<OnOffPair> m_upOnOffSensorPoints;
	unique_ptr<OnOffPair> m_upOnOffPerfMonMode;
	unique_ptr<OnOffPair> m_upOnOffColorMenu;
	unique_ptr<OnOffPair> m_upOnOffScanArea;
	unique_ptr<OnOffPair> m_upOnOffAskNotUndoable;
};