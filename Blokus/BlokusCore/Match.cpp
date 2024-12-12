// Match.cpp
//
// BlokusCore

module BlokusCore:Match;

import PerfCounter;
import :BlokusPreferences;

using std::wcout;
using std::endl;
using std::vector;

Match::Match()
{
    Initialize();
    Reset();
}

void Match::Reset()
{
    m_board.Reset();
    for (Player &player: m_players)
        player.Reset(); 
    m_protocol.Reset();
    m_idActivePlayer = FIRST_PLAYER;
    m_timer.Reset();
}

void Match::ResetTimers()
{
    for (Player &player: m_players)
        player.ResetTimer(); 
}

bool Match::AllShapeCellsUnblocked(BlokusMove const& move) const
{
    return move.GetShapeC().IsTrue4AllShapeCells
    (
        [this, &move](ShapeCoordPos const &cellPos)
        {
            CoordPos const  coordPos { move.GetCoordPos() + cellPos };
            Player   const &player   { GetPlayerC(move.GetPlayerId()) };
            return player.IsUnblockedPos(coordPos);
        }
    );
}

void Match::testPosition
(
    BlokusMove          &move, 
    ShapeCoordPos const &posCorner
) 
{ 
    ActivePlayerC().Apply2AllContactPntsC
    (
        [this, &move, &posCorner](CoordPos const& posContact)
        {
            move.SetCoordPos(posContact - posCorner);
            if (AllShapeCellsUnblocked(move))
            {
                m_validMoves.push_back(move);
            }
        }
	);
}

void Match::testShape(BlokusMove &move)
{
	GetShapeC(move).Apply2AllCornerPntsC
	(
		[this, &move](ShapeCoordPos const &pos) 
        { 
            testPosition(move, pos);
		}
	);
}

void Match::testPiece 
(
    BlokusMove  &move, 
    Piece const &piece
)
{
    PieceTypeId const pieceTypeId { piece.GetPieceTypeId() };
    PieceType   const pieceType   { Components::GetPieceTypeC(pieceTypeId) };
    move.SetPieceTypeId(pieceTypeId);
	pieceType.Apply2AllShapeIdsC
	(
		[this, &move](ShapeId const& id)
		{
            move.SetShapeId(id);
            testShape(move);
        }
    );
}

void Match::CalcListOfValidMoves() 
{
    BlokusMove move;
    m_validMoves.clear();
    move.SetPlayerId(m_idActivePlayer);
    ActivePlayer().Apply2AvailablePiecesC
    (
        [this, &move](Piece const& piece)
        {
            testPiece(move, piece);
        }
    );
}

//void Match::SetPosDir(BlokusMove const move)
//{
//	PosDir  posDirTarget { PosDir(Convert2fCoord(move.GetCoordPos()), 0._Degrees) };
//	PosDir &posDirAct    { GetPosDir(move) };
//	posDirAct = posDirTarget;
//}

    //g_iNrOfPieces = 0;
    //g_iNrOfShapes = 0;
    //g_iNrOfMoves  = 0;
    //m_timerFindValidMoves.BeforeAction();
    // 
    //m_timerFindValidMoves.AfterAction();
    //Ticks const ticks        { m_timerFindValidMoves.GetSingleActionTicks() };
    //Ticks const ticksPerMove {ticks / g_iNrOfMoves };
    //
    //wcout << L"GetListOfValidMoves:"                << endl;
    //wcout << g_iNrOfPieces       << L" pieces"      << endl;
    //wcout << g_iNrOfShapes       << L" shapes"      << endl;
    //wcout << g_iNrOfMoves        << L" moves"       << endl;
    //wcout << m_validMoves.size() << L" valid moves" << endl;
    //wcout << PerfCounter::Ticks2wstring(ticks) 
    //      << L"(" << PerfCounter::Ticks2wstring(ticksPerMove) << L" per move)" 
    //      << endl;
