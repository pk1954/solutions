// Player.cpp
//
// BlokusCore

module BlokusCore:Player;

import DrawContext;
import :BlokusUtilities;

using std::wstring;
using std::to_wstring;
using std::vector;

void Player::Initialize
(
    CoordPos   const  startPoint,
    Color      const  col,
	wstring    const &wstrColor,
	Strategy * const  pStrategy
)
{
    PieceTypeId id { 0 };
    Apply2AllPieces([&id](Piece& p){ p.Initialize(id++); });
	m_validPositions.Initialize();
	ClearContactPnts();
    AddContactPnt(startPoint);
    m_color           = col;
	m_wstrColor       = wstrColor;
	m_pStrategy       = pStrategy;
	m_pieceTypeIdMove = UndefinedPieceTypeId;
	m_remainingPieces = NR_OF_PIECE_TYPES;
    m_bFinished       = false;
    m_bFirstMove      = true;
}

void Player::DrawResult
(
	DrawContext     &context,
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
		L"Player " + m_wstrColor + L" finished with " + to_wstring(m_iResult) + L" points",
		hTextFormat
	);
}

void Player::DrawFreePieces(DrawContext &context) const
{
	Apply2FreePiecesC
	(
		[this, &context](Piece const& piece)
		{
			piece.GetPieceTypeC().Draw
			(
				context,
				Convert2fCoord(piece.GetPiecePos()),
				m_color
			);
		}
	);
}

void Player::DrawCell
(
	DrawContext &context,
	CoordPos    const &pos
) const
{
	ShapeSquare(context, pos, m_color );
}

void Player::DrawContactPnts(DrawContext &context) const
{
	Apply2AllContactPntsC
	(
		[this, &context](CoordPos const& pos)
		{
			SmallDot(context, pos, m_color);
		}
	);
}

void Player::reduceValidMoves
(
	Move      const &move,
	PieceType const &pieceType
)
{
    Shape const &shape { pieceType.GetShapeC(move.m_idShape)};
    shape.Apply2AllShapeCellsC
    (
        [this, &move](ShapeCoordPos const &shapePos)
        { 
            CoordPos const coordPos { move.m_boardPos + shapePos };
            m_validPositions.SetCell(coordPos, false);
            m_validPositions.SetCell(NorthPos(coordPos), false);
            m_validPositions.SetCell(EastPos (coordPos), false);
            m_validPositions.SetCell(SouthPos(coordPos), false);
            m_validPositions.SetCell(WestPos (coordPos), false);
        }
    );
    m_bFirstMove = false;
}

void Player::PerformMove(Move const& move)
{
	m_pieceTypeIdMove = move.m_idPieceType;
    PieceType const &pieceType { Components::GetPieceTypeC(m_pieceTypeIdMove) };
    Piece           &piece     { GetPiece(move.m_idPieceType) };
    piece.PerformMove(move);
    m_bFirstMove = false;
	if (--m_remainingPieces == 0)
	{
		m_bFinished = true;
	}
	else
	{
		reduceValidMoves(move, pieceType);
	}
}

void Player::DoFinish()
{
	m_bFinished = true;
	m_iResult = 0;
	Apply2FreePiecesC
	(
		[this](Piece const& piece)
		{
			PieceType const& pt { piece.GetPieceTypeC() };
			m_iResult -= pt.NrOfCells();
		}
	);
    if (m_remainingPieces == 0)
	{
		m_iResult = 15;
		if (Components::GetPieceTypeC(m_pieceTypeIdMove).NrOfCells() == 1)
			m_iResult += 5;
		m_bFinished = true;
	}
}
