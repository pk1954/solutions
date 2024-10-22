// Player.cpp
//
// BlokusCore

module BlokusCore:Player;

import IoConstants;
import DrawContext;
import :BlokusUtilities;

using std::wcout;
using std::endl;
using std::wstring;
using std::to_wstring;
using std::vector;

void Player::Initialize
(
    PlayerType const &type,
	Strategy * const  pStrategy
)
{
	m_pPlayerType = &type;
	m_pStrategy   = pStrategy;
	Reset();
}

void Player::Reset()
{
    PieceTypeId id { 0 };
    Apply2AllPieces([&id](Piece& p){ p.Reset(id++); });
	m_validPositions.Reset();
	ClearContactPnts();
	m_remainingPieces = NR_OF_PIECE_TYPES;
    m_bFinished       = false;
    m_bFirstMove      = true;
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

void Player::DrawFreePieces(DrawContext &context) const
{
	Apply2AvailablePiecesC
	(
		[this, &context](Piece const& piece)
		{
			PosDir const posDir { piece.GetPosDirC() };
			Color  const col    { m_pPlayerType->m_color };
			piece.GetPieceTypeC().Draw(context, ShapeId(0), posDir, col);
		}
	);
}

void Player::DrawCell
(
	DrawContext &context,
	CoordPos    const &pos
) const
{
	ShapeSquare(context, pos, m_pPlayerType->m_color );
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

void Player::reduceValidMoves(BlokusMove const &move)
{
	PieceType const &pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
    Shape     const &shape     { pieceType.GetShapeC(move.GetShapeId())};
    shape.Apply2AllShapeCellsC
    (
        [this, &move](ShapeCoordPos const &shapePos)
        { 
            CoordPos const coordPos { move.GetCoordPos() + shapePos };
            m_validPositions.SetCell(coordPos, false);
            m_validPositions.SetCell(NorthPos(coordPos), false);
            m_validPositions.SetCell(EastPos (coordPos), false);
            m_validPositions.SetCell(SouthPos(coordPos), false);
            m_validPositions.SetCell(WestPos (coordPos), false);
        }
    );
}

BlokusMove Player::SelectMove(RuleServerInterface const &rs)
{
	m_timer.BeforeAction();
    BlokusMove moveSelected { m_pStrategy->SelectMove(rs) };
    if (moveSelected.Undefined())
        finalize(moveSelected);    // no more valid moves
 	m_timer.AfterAction();
    return moveSelected;
}

void Player::PerformMove(BlokusMove &move)
{
    m_bFirstMove = false;
	if (--m_remainingPieces == 0)
	{
		finalize(move);     // all pieces set
	}
	else
	{
		reduceValidMoves(move);
	}
}

void Player::finalize(BlokusMove &move)
{
	m_bFinished = true;   
	m_iResult = 0;
	Apply2AvailablePiecesC
	(
		[this](Piece const& piece)
		{
			m_iResult -= piece.GetPieceTypeC().NrOfCells();
		}
	);
    if (m_remainingPieces == 0)
	{
		m_iResult += 15;
		if (Components::GetPieceTypeC(move.GetPieceTypeId()).NrOfCells() == 1)
			m_iResult += 5;
		move.Reset();
	}
}
