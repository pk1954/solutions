// BlokusAppMenu.ixx
//
// BlokusApp

export module BlokusAppMenu;

import std;
import ObserverInterface;
import WinBasics;
import OnOffPair;

using std::unique_ptr;

export class BlokusAppMenu : public ObserverInterface
{
public:
	~BlokusAppMenu() = default;

	void Initialize(HWND const);
	void Notify(bool const) final;

	HWND  m_hwndApp      { nullptr };
	HMENU m_hMenu        { nullptr };
	HMENU m_hMenuFile    { nullptr };
	HMENU m_hMenuView    { nullptr };
	HMENU m_hMenuOptions { nullptr };
	HMENU m_hMenuHelp    { nullptr };
};