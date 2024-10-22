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

    void GetListOfValidMoves(vector<BlokusMove> &validMoves) const final
    {
        m_match.FindValidMoves(validMoves);
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
    m_players.Initialize();
}

void Match::Reset()
{
    m_board.Reset();
    m_players.Apply2AllPlayers([](Player &p){ p.Reset(); });
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

BlokusMove Match::NextMove()
{
    BlokusMove move;
    Player    &player { ActivePlayer() };
    if (player.HasFinished())
        return move;
    move = player.SelectMove(*m_upRuleServer.get());  // may finish if no more valid moves
    if (move.Defined())
    {
        m_protocol.Add(move);
        player.PerformMove(move);                     // may finish, if all pieces set
    }
    if (player.HasFinished())                 // by one of two reasons
        --m_uiPlayersLeft;
    return move;
}

void Match::FinishMove(BlokusMove const move)
{
	Player &player { GetPlayer(move.GetPlayerId()) };
	Piece  &piece  { player.GetPiece(move.GetPieceTypeId()) };
	piece  .PerformMove(move);
    m_board.PerformMove(move);
}

void Match::NextPlayer()
{
    if (++m_idActivePlayer > LAST_PLAYER)
        m_idActivePlayer = FIRST_PLAYER;    
    findContactPnts();
}

bool Match::isValidMove
(
    BlokusMove const& move,
    Player     const& player
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

void Match::testPosition
(
    vector<BlokusMove>  &validMoves,
    BlokusMove          &move, 
    ShapeCoordPos const &posCorner
)
{ 
    Player const& player { m_players.GetPlayerC(move.GetPlayerId()) };
    player.Apply2AllContactPntsC
    (
        [this, &validMoves, &player, &move, &posCorner](CoordPos const& posContact)
        {
            move.SetCoordPos(posContact - posCorner);
            if (isValidMove(move, player))
            {
                validMoves.push_back(move);
            }
        }
	);
}

void Match::testShape
(
    vector<BlokusMove> &validMoves,
    BlokusMove         &move, 
    ShapeId const      idShape
)
{
    //++g_iNrOfShapes;
    move.SetShapeId(idShape);
	GetShapeC(move).Apply2AllCornerPntsC
	(
		[this, &validMoves, &move](ShapeCoordPos const &pos) 
        { 
            testPosition(validMoves, move, pos);
		}
	);
}

void Match::testPiece
(
    vector<BlokusMove> &validMoves,
    BlokusMove         &move, 
    Piece const        &piece
)
{
    PieceTypeId const pieceTypeId { piece.GetPieceTypeId() };
    PieceType   const pieceType   { Components::GetPieceTypeC(pieceTypeId) };
    move.SetPieceTypeId(pieceTypeId);
    //++g_iNrOfPieces;
	pieceType.Apply2AllShapeIdsC
	(
		[this, &validMoves, &move](ShapeId const& id)
		{
            testShape(validMoves, move, id);
        }
    );
}

void Match::FindValidMoves(vector<BlokusMove> &validMoves)
{
    Player     player { ActivePlayer() };
    BlokusMove move;
    validMoves.clear();
    move.SetPlayerId(m_idActivePlayer);
    player.Apply2AvailablePiecesC
    (
        [this, &validMoves, &move](Piece const& piece)
        {
            testPiece(validMoves, move, piece);
        }
    );
}

    //g_iNrOfPieces = 0;
    //g_iNrOfShapes = 0;
    //g_iNrOfMoves  = 0;
    //m_timerFindValidMoves.BeforeAction();
    // 
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
