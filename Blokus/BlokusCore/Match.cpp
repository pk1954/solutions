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
    Initialize();
}

void Match::Reset()
{
    m_board.Reset();
    Apply2AllPlayers([](Player &p){ p.Reset(); });
    m_protocol.Reset();
    m_idActivePlayer = FIRST_PLAYER;
    findContactPnts();
    m_timer.Reset();
}

void Match::ResetTimers()
{
    Apply2AllPlayers([](Player &player) { player.ResetTimer(); });
}

//void Match::SetPosDir(BlokusMove const move)
//{
//	PosDir  posDirTarget { PosDir(Convert2fCoord(move.GetCoordPos()), 0._Degrees) };
//	PosDir &posDirAct    { GetPosDir(move) };
//	posDirAct = posDirTarget;
//}

Piece& Match::GetPiece(BlokusMove const move)
{
	Player &player { GetPlayer(move.GetPlayerId()) };
	return player.GetPiece(move.GetPieceTypeId());
}

Piece const &Match::GetPieceC(BlokusMove const move) const
{
	Player const &player { GetPlayerC(move.GetPlayerId()) };
	return player.GetPieceC(move.GetPieceTypeId());
}

PieceType const &Match::GetPieceTypeC(BlokusMove const move) const
{
	Piece     const &piece     { GetPieceC(move) };
	PieceType const &pieceType { piece.GetPieceTypeC() };
	return pieceType;
}

Degrees Match::GetRotation(BlokusMove const move) const
{
	Piece     const &piece     { GetPieceC(move) };
	PieceType const &pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
	Shape     const &shape     { pieceType.GetShapeC(move.GetShapeId()) };
	return shape.GetRotation();
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
                Player const& player { GetPlayerC(idPlayer) };
                player.DrawCell(context, pos);
		    }
        }
    );
}

void Match::DrawFreePieces
(
    DrawContext     &context,
    BlokusMove const move
) const
{
	if (m_idActivePlayer == NO_PLAYER)
        return;
    Player const &player { ActivePlayerC() };
	if (move.IsDefined())
	{
        static Color const COL_GREY { 0.5f, 0.5f, 0.5f, 0.5f };
        PieceType     const &pieceType   { move.GetPieceTypeC() };
		MicroMeterPnt const  umPosTarget { Convert2fCoord(move.GetCoordPos()) };
		Color         const  color       { IsValidPosition(move) ? player.GetColor() : COL_GREY};
		pieceType.Draw(context, ShapeId(0), umPosTarget, color, false);
	}
	{
        PieceTypeId const  idPieceType   { move.GetPieceTypeId() };
        Piece       const *pPieceSelected 
        { 
            (IsValidPieceTypeId(idPieceType)) 
            ? &player.GetPieceC(idPieceType) 
            : nullptr
        };
	    player.DrawFreePieces(context, pPieceSelected);
	}
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

BlokusMove Match::DoMove(BlokusMove move)
{
    ActivePlayer().DoMove(move);     // may finish, if all pieces set
    if (move.IsDefined())
    {
	    GetPiece(move).DoMove(move.GetCoordPos());
        m_board.DoMove(move);
    }
    move.SetPlayerId(NextPlayer());
    return move;
}

BlokusMove Match::DoMove()
{
    Player    &player { ActivePlayer() };
    BlokusMove move   { player.SelectMove(*m_upRuleServer.get()) };  // may finish if no more valid moves
    move = DoMove(move);        
    return move;
}

void Match::UndoMove(BlokusMove const move)
{
    if (move.IsDefined())
    {
	    GetPiece(move).Reset();
        m_board.UndoMove(move);
    }
    m_idActivePlayer = move.GetPlayerId();
}

PlayerId Match::NextPlayer()
{
    if (++m_idActivePlayer > LAST_PLAYER)
        m_idActivePlayer = FIRST_PLAYER;    
    findContactPnts();
    return m_idActivePlayer;
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

bool Match::HasContact(BlokusMove const& move) const
{
    return move.GetShapeC().IsTrueForAnyCornerPnt
    (
        [this, &move](ShapeCoordPos const& shapePosCorner)
        {
            return ActivePlayerC().IsTrue4AnyContactPnt
            (
                [&shapePosCorner, &move](CoordPos const &posContactPn)
                {
                    return move.GetCoordPos() + shapePosCorner == posContactPn;
                }
            );
        }
    );
}

bool Match::IsNotBlocked(BlokusMove const& move) const
{
    return move.GetShapeC().IsTrue4AllShapeCells
    (
        [this, &move](ShapeCoordPos const &shapePos)
        {
            CoordPos const  coordPos { move.GetCoordPos() + shapePos };
            Player   const &player   { GetPlayerC(move.GetPlayerId()) };
            return player.IsUnblockedPos(coordPos);
        }
    );
}

bool Match::IsCompletelyOnBoard(BlokusMove const& move) const
{
    CoordPos const& pos   { move.GetCoordPos() };
    Shape    const& shape { move.GetShapeC  () };
    return shape.IsCompletelyOnBoard(pos);
}

bool Match::IsValidPosition(BlokusMove const& move) const
{
    return IsCompletelyOnBoard(move) && IsNotBlocked(move) && HasContact(move);
}

void Match::testPosition
(
    vector<BlokusMove>  &validMoves,
    BlokusMove          &move, 
    ShapeCoordPos const &posCorner
)
{ 
    Player const& player { GetPlayerC(move.GetPlayerId()) };
    player.Apply2AllContactPntsC
    (
        [this, &validMoves, &player, &move, &posCorner](CoordPos const& posContact)
        {
            move.SetCoordPos(posContact - posCorner);
            if (IsNotBlocked(move))
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

bool Match::AllFinished() const
{
    bool bResult = true;
    Apply2AllPlayersC
    (
        [&bResult](Player const& player)
        {
            if (!player.HasFinished())
                bResult = false;
        }
    );
    return bResult;
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
