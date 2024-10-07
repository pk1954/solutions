// Match.cpp
//
// BlokusCore

module BlokusCore:Match;

import PerfCounter;

using std::wcout;
using std::endl;
using std::vector;
using std::make_unique;

class RuleServer : public RuleServerInterface
{
public:
    RuleServer(Match &match)
        : m_match(match)
    {}

    vector<Move> const& GetListOfValidMoves() const final
    {
        return m_match.FindValidMoves();
    }

    Board const& GetBoard() const final
    {
        return m_match.GetBoard();
    }

private:
    Match &m_match;
};

Match::Match()
{
    m_upRuleServer = make_unique<RuleServer>(*this);
}

void Match::Reset()
{
    m_board.Reset();
    m_players.Reset();
    m_protocol.Reset();
    m_uiPlayersLeft  = NR_OF_PLAYERS;
    m_idActivePlayer = FIRST_PLAYER;
    findContactPnts();
    m_timer.Reset();
}

void Match::DrawSetPieces(DrawContext &context) const
{
    Apply2AllBoardCells
    (
        [this, &context](CoordPos const& pos)
        {
		    PlayerId const idPlayer { m_board.GetPlayerId(pos) };
		    if (idPlayer != NO_PLAYER)
		    {
                Player const& player { m_players.GetPlayerC(idPlayer) };
                player.DrawCell(context, pos);
		    }
        }
    );
}

PlayerId Match::WinnerId() const
{
    PlayerId idBestPlayer { NO_PLAYER };
    int      iBestResult  { (std::numeric_limits<int>::min)() };
    Apply2AllPlayerIds
    (
        [this, &idBestPlayer, &iBestResult](PlayerId const idPlayer)
        {
            Player const &player { m_players.GetPlayerC(idPlayer) };
            if (player.Result() > iBestResult)
            {
                iBestResult = player.Result();
                idBestPlayer = idPlayer;
            }
        }
    );
    return idBestPlayer;
}

void Match::NextMove()
{
    Player &player { ActivePlayer() };
    if (!player.HasFinished())
    {
        Move move { player.SelectMove(*m_upRuleServer.get()) };  // may finish if no more valid moves
        if (move.Defined())
        {
            m_protocol.Add(move);
            player.PerformMove(move);
            m_board.PerformMove(move);  // may finish, if all pieces set
        }
        if (player.HasFinished())       // by one of two reasons
            --m_uiPlayersLeft;
    }
    if (++m_idActivePlayer > LAST_PLAYER)
        m_idActivePlayer = FIRST_PLAYER;    
    findContactPnts();
}

//Ticks const ticks { m_timer.GetSingleActionTicks() };
//wcout << L"complete move:"<< PerfCounter::Ticks2wstring(ticks) << endl;
//if (MatchFinished())
//{
//    m_timer.AfterAction();
//	Ticks const ticks { m_timer.GetSingleActionTicks() };
//	wcout << L"Complete match:"<< PerfCounter::Ticks2wstring(ticks) << endl;
//}
//Ticks const ticks { m_timer.GetAccumulatedActionTicks() };
//wcout << L"all moves:"<< PerfCounter::Ticks2wstring(ticks) << endl;

bool Match::isValidMove
(
    Move   const& move,
    Player const& player
)
{
    PieceType const& pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
    Shape     const& shape     { pieceType  .GetShapeC    (move.GetShapeId()) };
    return shape.Apply2AllShapeCellsB
    (
        [this, &move, &player](ShapeCoordPos const &shapePos)
        {
            CoordPos const coordPos { move.GetCoordPos() + shapePos };
            return m_board.IsFreeCell(coordPos) && player.IsValidPos(coordPos);
        }
    );
}

void Match::findContactPnts()
{
    Player &player { ActivePlayer() };
    player.ClearContactPnts();
    if (player.IsFirstMove())
    {
        player.AddInitialContactPnt();
    }
    else
    {
        Apply2AllBoardCells
        (
            [this, &player](CoordPos const& pos)
            {
                if (m_board.IsContactPnt(pos, m_idActivePlayer))
                {
                    player.AddContactPnt(pos);
                }
            }
        );
    }
}

vector<Move> const & Match::FindValidMoves()
{
    g_iNrOfPieces = 0;
    g_iNrOfShapes = 0;
    g_iNrOfMoves  = 0;
    m_timerFindValidMoves.BeforeAction();
    Player player { ActivePlayer() };
    Move   move;
    m_validMoves.clear();
    move.SetPlayerId(m_idActivePlayer);
    player.Apply2FreePiecesC
    (
        [this, &move](Piece const& piece)
        {
            move.SetPieceTypeId(piece.GetPieceTypeId());
            ++g_iNrOfPieces;
		    Components::GetPieceTypeC(move.GetPieceTypeId()).Apply2AllShapeIdsC
		    (
			    [this, &move](ShapeId const& idShape)
			    {
                    move.SetShapeId(idShape);
                    ++g_iNrOfShapes;
				    GetShapeC(move).Apply2AllCornerPntsC
				    (
					    [this, &move](ShapeCoordPos const &posCorner) 
                        { 
                            Player const& player { m_players.GetPlayerC(move.GetPlayerId()) };
                            player.Apply2AllContactPntsC
                            (
                                [this, &player, &move, &posCorner](CoordPos const& posContact)
                                {
                                    move.SetCoordPos(posContact - posCorner);
                                    ++g_iNrOfMoves;
                                    if (isValidMove(move, player))
                                    {
                                        m_validMoves.push_back(move);
                                    }
                                }
				            );
			            }
		            );
                }
            );
        }
    );

    //m_timerFindValidMoves.AfterAction();
    //Ticks const ticks        { m_timerFindValidMoves.GetSingleActionTicks() };
    //Ticks const ticksPerMove {ticks / g_iNrOfMoves };
    //
    //wcout << L"FindValidMoves:"                     << endl;
    //wcout << g_iNrOfPieces       << L" pieces"      << endl;
    //wcout << g_iNrOfShapes       << L" shapes"      << endl;
    //wcout << g_iNrOfMoves        << L" moves"       << endl;
    //wcout << m_validMoves.size() << L" valid moves" << endl;
    //wcout << PerfCounter::Ticks2wstring(ticks) 
    //      << L"(" << PerfCounter::Ticks2wstring(ticksPerMove) << L" per move)" 
    //      << endl;

    return m_validMoves;
}
