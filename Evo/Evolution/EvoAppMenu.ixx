// EvoAppMenu.ixx
//
// Evolution

export module EvoAppMenu;

import WinBasics;
import WorkThread;
import WinManager;
import AppMenu;

class EvoAppMenu : public AppMenu
{
public:
	EvoAppMenu() :
		m_hMenu(nullptr),
		m_pWorkThread(nullptr),
		m_pWinManager(nullptr)
	{}

	virtual void Initialize(HWND const, WorkThread const * const, WinManager const * const);
	virtual void AdjustVisibility();
	virtual void Start();
	virtual void Stop();

private:
	void enableMenues(UINT const);

	HMENU              m_hMenu;
	WorkThread const * m_pWorkThread;
	WinManager const * m_pWinManager;
};
