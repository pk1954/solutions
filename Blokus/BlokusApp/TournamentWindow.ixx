// TournamentWindow.ixx
//
// BlokusApp

export module TournamentWindow;

import std;
import HiResTimer;
import BaseWindow;
import BlokusCore;
import TournamentTable;
import TournamentThreadManager;

using std::array;
using std::vector;
using std::wstring;
using std::unique_ptr;

export class TournamentWindow : public BaseWindow
{
public:
    TournamentWindow(HWND const);

	void DoGameStuff();

private:
    inline static int const ID_FIRST_COMBO_BOX { 1000 };
    
    bool isComboBoxId(int const id)
    {
        return (ID_FIRST_COMBO_BOX <= id) && (id < ID_FIRST_COMBO_BOX + NR_OF_PLAYERS);
    }

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
    bool OnSize(PIXEL const, PIXEL const) final;

    array<HWND, NR_OF_PLAYERS> m_comboBox;
    vector<wstring const*>     m_strategyNameList;

    TournamentTable        m_table;
    unique_ptr<Tournament> m_upTournament;
};
