// Player.cpp
//
// BlokusCore

module BlokusCore:Player;

import :Util;

using std::wstring;
using std::to_wstring;
using std::vector;

void Player::Initialize
(
    CoordPos const  startPoint,
    Color    const  col,
	wstring  const &wstrColor
)
{
    PieceTypeId id { 0 };
    Apply2AllPieces([&id](Piece& p){ p.Initialize(id++); });
	m_validPositions.Initialize();
	ClearContactPnts();
    AddContactPnt(startPoint);
    m_color           = col;
	m_wstrColor       = wstrColor;
	m_pieceTypeIdMove = UndefinedPieceTypeId;
	m_remainingPieces = NR_OF_PIECE_TYPES;
    m_bFinished       = false;
    m_bFirstMove      = true;
}

void Player::DrawResult
(
	D2D_driver       const& d2d,
	BlokusCoordSys   const& coordSys,
	TextFormatHandle const  hTextFormat
) const
{
	fPixelRect fPixRect 
	{ 
		coordSys.Transform2fPixelPos(CoordPos(COORD_BOARD_SIZE,            -1_COORD)),
		coordSys.Transform2fPixelPos(CoordPos(COORD_BOARD_SIZE + 13_COORD,  0_COORD))
	};
	d2d.DisplayText
	(
		fPixRect, 
		L"Player " + m_wstrColor + L" finished with " + to_wstring(m_iResult) + L" points",
		hTextFormat
	);
}

void Player::DrawFreePieces
(
	D2D_driver const &d2d,
	BlokusCoordSys   &coordSys
) const
{
	Apply2FreePiecesC
	(
		[this, &d2d, &coordSys](Piece const& piece)
		{
			piece.GetPieceTypeC().Draw
			(
				d2d,
				coordSys,
				piece.GetPos(),
				m_color
			);
		}
	);
}

void Player::DrawCell
(
	D2D_driver const &d2d,
	BlokusCoordSys   &coordSys,
	CoordPos   const &pos
) const
{
	ShapeSquare(d2d, coordSys, pos, m_color );
}

void Player::DrawContactPnts
(
	D2D_driver const &d2d,
	BlokusCoordSys   &coordSys
) const
{
	Apply2AllContactPntsC
	(
		[this, &d2d, &coordSys](CoordPos const& pos)
		{
			SmallDot(d2d, coordSys, pos, m_color);
		}
	);
}

Move const& Player::SelectMove(vector<Move> const& moves) const
{
    //unsigned int uiMax { Cast2UnsignedInt(moves.size()) - 1 };
    //unsigned int uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
    //return moves[uiSel];
    return moves[0];
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
