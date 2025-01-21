// TournamentWindow.ixx
//
// BlokusApp

export module TournamentWindow;

import std;
import HiResTimer;
import TableWindow;
import BlokusCore;
import TournamentThreadManager;

using std::array;
using std::vector;
using std::wstring;
using std::unique_ptr;

export class TournamentWindow : public TableWindow
{
public:
    TournamentWindow(HWND const);

    void PaintGraphics() final;

private:
    inline static int const ID_FIRST_COMBO_BOX { 1000 };

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;

    array<HWND, NR_OF_PLAYERS> m_comboBox;
    vector<wstring const*>     m_strategyNameList;

    unique_ptr<Tournament> m_upTournament;

    HiResTimer m_timer;
    int        m_iNrOfMatches;
    int        m_iNrOfThreads;
    int        m_iMatch;
    bool       m_bActive;
};
