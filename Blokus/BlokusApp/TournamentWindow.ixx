// TournamentWindow.ixx
//
// BlokusApp

export module TournamentWindow;

import std;
import TableWindow;
import BlokusCore;

using std::to_wstring;

export class TournamentWindow : public TableWindow
{
public:

    void Start(Tournament const *pTournament)
    {
        m_pTournament = pTournament;
        Resize(RasterPoint(4,7));
    }

    void PaintGraphics() final
    {
        Ticks ticksAllPlayers { 0 };
        TableWindow::Clear();
        Set(RasterPoint(0, 0), to_wstring(m_pTournament->MatchNumber()) +
                               L"/" +
                               to_wstring(m_pTournament->NrOfMatches()));
        Set(RasterPoint(1, 0), L"Strategy");
        Set(RasterPoint(2, 0), L"#Wins");
        Set(RasterPoint(3, 0), L"Time");

        Apply2AllPlayerIds
        (
            [this, &ticksAllPlayers](PlayerId id)
            {
                Ticks const ticksPlayer { m_pTournament->GetTimeUsed(id) };
                int   const yPos        { id.GetValue() + 1 };
                Set(RasterPoint(0, yPos), PlayerTypes::GetPlayerType(id).m_wstrName);
                Set(RasterPoint(1, yPos), m_pTournament->GetStrategyName(id));
                Set(RasterPoint(2, yPos), m_pTournament->NrOfWins(id));
                Set(RasterPoint(3, yPos), PerfCounter::Ticks2wstring(ticksPlayer));
                ticksAllPlayers += ticksPlayer;
            }
        );

        Ticks ticksTournament { m_pTournament->GetTournamentTime() };
        Ticks ticksOverhead   { ticksTournament - ticksAllPlayers };
        Set(RasterPoint(2, 5), L"overhead");
        Set(RasterPoint(3, 5), PerfCounter::Ticks2wstring(ticksOverhead));
        Set(RasterPoint(2, 6), L"tournament");
        Set(RasterPoint(3, 6), PerfCounter::Ticks2wstring(ticksTournament));

        TableWindow::CalcRowsAndCols();
        TableWindow::PaintGraphics();
    }

private:
    Tournament const * m_pTournament { nullptr };
};
