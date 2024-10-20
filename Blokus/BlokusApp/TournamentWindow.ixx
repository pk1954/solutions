// TournamentWindow.ixx
//
// BlokusApp

export module TournamentWindow;

import HiResTimer;
import TableWindow;
import BlokusCore;

export class TournamentWindow : public TableWindow
{
public:

    void Start(Tournament const *);
    void PaintGraphics() final;

private:
    Tournament const * m_pTournament { nullptr };
    HiResTimer         m_timer;
};
