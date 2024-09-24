// BlokusAppMenu.ixx
//
// Blokus

export module BlokusAppMenu;

import std;
import ObserverInterface;
import WinBasics;
import OnOffPair;

using std::unique_ptr;

export class BlokusAppMenu : public ObserverInterface
{
public:
	BlokusAppMenu();
	~BlokusAppMenu() = default;

	void Start(HWND const);
	void Notify(bool const) final;

private:

	HWND  m_hwndApp { nullptr };
	HMENU m_hMenu   { nullptr };

	unique_ptr<OnOffPair> m_upOnOffShowContactPnts;
	unique_ptr<OnOffPair> m_upOnOffShowCornerCells;
};