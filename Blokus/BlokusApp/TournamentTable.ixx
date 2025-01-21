// TournamentTable.ixx
//
// BlokusApp

export module TournamentTable;

import WinBasics;
import TableWindow;
import BlokusCore;

export class TournamentTable : public TableWindow
{
public:

    void Initialize(HWND const, Tournament * const);


private:

    Tournament * m_pTournament;
    HiResTimer   m_timer;

    void PaintGraphics() final;
};