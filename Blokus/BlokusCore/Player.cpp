// Player.cpp
//
// BlokusCore

module BlokusCore:Player;

import Debug;
import IoConstants;
import DrawContext;
import :BlokusUtilities;
import :Board;

using std::wcout;
using std::endl;
using std::wstring;
using std::to_wstring;
using std::vector;

void Player::Initialize
(
	Board      const &board,
	PlayerId   const  id,
	Strategy * const  pStrategy
)
{
	m_pBoard      = &board;
	m_pPlayerType = &Components::GetPlayerType(id);
	m_idPlayer    = id;
	m_pStrategy   = pStrategy;
	Reset();
}

void Player::Reset()
{
    PieceTypeId id { 0 };
    Apply2AllPieces([&id](Piece& p){ p.Initialize(id++); });
	m_remainingPieces = NR_OF_PIECE_TYPES;
    m_bFinished       = false;
    m_bFirstMove      = true;
	m_mapOfValidCells.Reset();
	recalcListOfContactPnts();
	recalcMapOfValidCells();
}

void Player::DrawResult
(
	DrawContext           &context,
	TextFormatHandle const hTextFormat
) const
{
	MicroMeterRect rect
	{ 
		MicroMeterPnt(UM_BOARD_SIZE,                       -UM_CELL_SIZE),
		MicroMeterPnt(UM_BOARD_SIZE + UM_CELL_SIZE * 13.f, 0._MicroMeter)
	};
	context.DisplayText
	(
		rect, 
		L"Player "         + m_pPlayerType->m_wstrName + 
		L" finished with " + to_wstring(m_iResult) + 
		L" points",
		hTextFormat
	);
}

void Player::DrawFreePieces
(
	DrawContext        &context,
	Piece const * const pPieceNoDraw
) const
{
	Apply2AvailablePiecesC
	(
		[this, &context, pPieceNoDraw](Piece const& piece)
		{
			if (&piece != pPieceNoDraw)
			{
				PosDir const posDir { piece.GetPosDirC() };
				Color  const col    { m_pPlayerType->m_color };
				piece.Draw(context, posDir, col, false);
			}
		}
	);
}

void Player::DrawSetPieces(DrawContext &context) const
{
	Apply2SetPiecesC
	(
		[this, &context](Piece const& piece)
		{
			PosDir const posDir { piece.GetPosDirC() };
			Color  const col    { m_pPlayerType->m_color };
			piece.Draw(context, posDir, col, false);
		}
	);
}

void Player::DrawContactPnts(DrawContext &context) const
{
	Apply2AllContactPntsC
	(
		[this, &context](CoordPos const& pos)
		{
			SmallDot(context, pos, m_pPlayerType->m_color);
		}
	);
}

void Player::blockPosition(CoordPos const &coordPos)
{
	if (IsOnBoard(coordPos))
		m_mapOfValidCells.BlockCell(coordPos);
}

void Player::blockNeighbours(CoordPos const &coordPos)
{
    blockPosition(NorthPos(coordPos));
    blockPosition(EastPos (coordPos));
    blockPosition(SouthPos(coordPos));
    blockPosition(WestPos (coordPos));
}

void Player::recalcListOfContactPnts()
{
    m_contactPntsOnBoard.clear();
    if (IsFirstMove())
    {
        m_contactPntsOnBoard.push_back(m_pPlayerType->m_startPoint);
    }
    else
    {
        Apply2AllBoardCells
        (
            [this](CoordPos const& pos)
            {
                if (m_pBoard->IsContactPnt(pos, m_idPlayer))
                {
                    m_contactPntsOnBoard.push_back(pos);
                }
            }
        );
    }
}

bool Player::AnyShapeCellsBlocked(BlokusMove const& move) const
{
    return move.GetShapeC().IsTrue4AnyShapeCell
    (
        [this, &move](ShapeCoordPos const &cellPos)
        {
            return IsBlocked(move.GetCoordPos() + cellPos);
        }
    );
}

void Player::testPosition
(
    BlokusMove          &move, 
    ShapeCoordPos const &posCorner
) 
{ 
    Apply2AllContactPntsC
    (
        [this, &move, &posCorner](CoordPos const& posContact)
        {
            move.SetCoordPos(posContact - posCorner);
            if (AnyShapeCellsBlocked(move))
                return;
             m_validMoves.push_back(move);
             Assert(!AnyShapeCellsBlocked(move));
        }
	);
}

void Player::testShape(BlokusMove &move)
{
	GetShapeC(move).Apply2AllCornerPntsC
	(
		[this, &move](ShapeCoordPos const &pos) 
        { 
            testPosition(move, pos);
		}
	);
}

void Player::testPiece 
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

void Player::recalcMapOfValidCells()
{
	Apply2AllBoardCells
	(
		[this](CoordPos const& coordPos)
		{
			PlayerId idPlayer { m_pBoard->GetPlayerId(coordPos) };
			if (idPlayer != NO_PLAYER)
				blockPosition(coordPos);
			if (idPlayer == m_idPlayer)
				blockNeighbours(coordPos);
		}
	);
}

void Player::calcListOfValidMoves()
{
    BlokusMove move;
    move.SetPlayerId(m_idPlayer);
	m_validMoves.clear();
    Apply2AvailablePiecesC
    (
        [this, &move](Piece const& piece)
        {
            testPiece(move, piece);
        }
    );
	if (m_validMoves.empty())
		Finalize();
}

void Player::CheckListOfValidMoves() 
{
    for (BlokusMove const& move : m_validMoves)
        Assert(!AnyShapeCellsBlocked(move));
}

void Player::Prepare()
{
	recalcListOfContactPnts();
	recalcMapOfValidCells();
	calcListOfValidMoves();
    CheckListOfValidMoves();
}

BlokusMove Player::SelectMove(RuleServerInterface const &rsi) const
{
	m_timer.BeforeAction();
	BlokusMove moveSelected { m_pStrategy->SelectMove(rsi) };
 	m_timer.AfterAction();
    return moveSelected;
}

void Player::DoMove(BlokusMove &move)
{
    Assert(move.IsDefined());
	if (--m_remainingPieces == 0)
	{
		Finalize();     // all pieces set
		m_iResult += 15;
		if (Components::GetPieceTypeC(move.GetPieceTypeId()).NrOfCells() == 1)
			m_iResult += 5;
	}
	m_bFirstMove = false;
}

void Player::UndoMove()
{
	m_bFinished = false;   
	m_iResult   = 0;
	++m_remainingPieces;
	if (m_remainingPieces == NR_OF_PIECE_TYPES)
		m_bFirstMove = true;
}

void Player::Finalize()
{
	m_bFinished = true;   
	m_iResult   = 0;
	Apply2AvailablePiecesC
	(
		[this](Piece const& piece)
		{
			m_iResult -= piece.GetPieceTypeC().NrOfCells();
		}
	);
}

void Player::UndoFinalize()
{
	m_bFinished = false;   
	m_iResult   = 0;
}
