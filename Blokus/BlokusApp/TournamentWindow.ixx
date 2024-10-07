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
        Resize(RasterPoint(4,5));
    }

    void PaintGraphics() final
    {
        TableWindow::Clear();
        Set(RasterPoint(0, 0), to_wstring(m_pTournament->MatchNumber()) +
                               L"/" +
                               to_wstring(m_pTournament->NrOfMatches()));
        Set(RasterPoint(1, 0), L"Strategy");
        Set(RasterPoint(2, 0), L"#Wins");
        Set(RasterPoint(3, 0), L"Time");

        Apply2AllPlayerIds
        (
            [this](PlayerId id)
            {
                Set(RasterPoint(0, id.GetValue() + 1), PlayerTypes::GetPlayerType(id).m_wstrName);
                Set(RasterPoint(2, id.GetValue() + 1), m_pTournament->NrOfWins(id));
            }
        );
        TableWindow::CalcRowsAndCols();
        TableWindow::PaintGraphics();
    }

private:
    Tournament const * m_pTournament { nullptr };
};
