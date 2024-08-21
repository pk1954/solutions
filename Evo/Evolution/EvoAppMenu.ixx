// EvoAppMenu.ixx
//
// Evolution

export module EvoAppMenu;

import AppMenu;

class EvoAppMenu : public AppMenu
{
public:
	EvoAppMenu() :
		m_hMenu(nullptr),
		m_pWorkThreadInterface(nullptr),
		m_pWinManager(nullptr)
	{}

	virtual void Initialize(HWND const, WorkThreadInterface const * const, WinManager const * const);
	virtual void AdjustVisibility();
	virtual void Start();
	virtual void Stop();

private:
	void enableMenues(UINT const);

	HMENU                       m_hMenu;
	WorkThreadInterface const * m_pWorkThreadInterface;
	WinManager          const * m_pWinManager;
};
