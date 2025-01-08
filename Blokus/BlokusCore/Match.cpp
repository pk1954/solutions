// Match.cpp
//
// BlokusCore

module BlokusCore:Match;

import PerfCounter;

using std::wcout;
using std::endl;
using std::vector;

Match::Match()
{
    m_players[0].Initialize(m_board, PlayerId(0), &StrategyRed);
    m_players[1].Initialize(m_board, PlayerId(1), &StrategyGreen); 
    m_players[2].Initialize(m_board, PlayerId(2), &StrategyBlue);
    m_players[3].Initialize(m_board, PlayerId(3), &StrategyYellow);
    for (int i = 0; i < NR_OF_PLAYERS; ++i)
    	m_board.RegisterObserver(m_players[i]);
    Reset();
}

void Match::Dump() const
{
    m_board.Dump();
    Apply2AllPlayersC([](Player const &p) { p.Dump(); });
    wcout << endl;
}

void Match::Reset()
{
    m_board.Reset();
    for (Player &player: m_players)
        player.Reset(); 
    m_protocol.Reset();
    m_timer.Reset();
}

void Match::ResetTimers()
{
    for (Player &player: m_players)
        player.ResetTimer(); 
}

bool Match::AnyShapeCellsBlocked(BlokusMove const move) const
{
	Player const &player { GetPlayerC(move) };
    return player.AnyShapeCellsBlocked(move);
}

CoordPos Match::FindBestFit(BlokusMove const move) const
{
    CoordPos posShapeTargetBest { UndefinedCoordPos };
	int      iMinDistSquare     { 400 };

    Shape    const &shape         { move.GetShapeC() };
	Player   const &player        { GetPlayerC(move) };
    CoordPos const  posShapeMouse { move.GetCoordPos() };  // shape position by mouse move
    player.Apply2AllContactPntsC
    (
        [&shape, &posShapeTargetBest, &player, &posShapeMouse, &iMinDistSquare](CoordPos const& posContact)
        {
            shape.Apply2AllCornerPntsC
            (
                [&posContact, &posShapeTargetBest, &player, &posShapeMouse, &iMinDistSquare, &shape](CoordPos const& posCornerRel)
                {
                    CoordPos const posShapeCandidate { posContact - posCornerRel };  // posCornerAbsolute == posContact
                    if (!player.AnyShapeCellsBlocked(shape, posShapeCandidate))
                    {
			            int iDistSquare { DistSquare(posShapeCandidate, posShapeMouse) };
                        if (iDistSquare < iMinDistSquare)
                        {
				            iMinDistSquare     = iDistSquare;
                            posShapeTargetBest = posShapeCandidate;
                        }
                    }
                }
            );
        }
    );

    return posShapeTargetBest;
}

void Match::DoMove(BlokusMove const move)
{
    Assert(move.IsDefined());
    Player &player { GetPlayer(move) };
    Assert(!player.HasFinished());
	GetPiece(move).DoMove(move);     // Set piece pos and mark as set
    m_board.DoMove(move);            // Set affected cells to player id
    player.DoMove(move);             // Set remaining pieces, etc. May finish, if all pieces set
}

PlayerId Match::WinnerId() const
{
    PlayerId idBestPlayer { NO_PLAYER };
    int      iBestResult  { (std::numeric_limits<int>::min)() };
    Apply2AllPlayerIds
    (
        [this, &idBestPlayer, &iBestResult](PlayerId const idPlayer)
        {
            Player const &player { GetPlayerC(idPlayer) };
            if (player.Result() > iBestResult)
            {
                iBestResult = player.Result();
                idBestPlayer = idPlayer;
            }
        }
    );
    return idBestPlayer;
}

