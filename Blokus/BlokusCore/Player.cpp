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
	m_mapOfValidCells.Reset();
	RecalcListOfContactPnts();
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
				piece.GetPieceTypeC().Draw(context, ShapeId(0), posDir, col, false);
			}
		}
	);
}

void Player::DrawCell
(
	DrawContext    &context,
	CoordPos const &pos
) const
{
	ShapeSquare(context, pos, m_pPlayerType->m_color, false);
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
    m_mapOfValidCells.SetCell(coordPos, false);
}

void Player::blockNeighbours(CoordPos const &coordPos)
{
    blockPosition(coordPos);
    blockPosition(NorthPos(coordPos));
    blockPosition(EastPos (coordPos));
    blockPosition(SouthPos(coordPos));
    blockPosition(WestPos (coordPos));
}

void Player::reduceValidPositions
(
	Shape    const &shape,
	CoordPos const &coordPos
)
{
    shape.Apply2AllShapeCellsC
    (
        [this, &coordPos](ShapeCoordPos const &shapePos)
        { 
            blockNeighbours(coordPos + shapePos);
        }
    );
}

void Player::RecalcListOfContactPnts()
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
	m_bFirstMove = false;
	if (--m_remainingPieces == 0)
	{
		Finalize();     // all pieces set
		m_iResult += 15;
		if (Components::GetPieceTypeC(move.GetPieceTypeId()).NrOfCells() == 1)
			m_iResult += 5;
	}
	else
	{
		reduceValidPositions(move.GetShapeC(), move.GetCoordPos());
	}
}

void Player::recalcMapOfValidCells()
{
	Apply2AllBoardCells
	(
		[this](CoordPos const& coordPos)
		{
			PlayerId idPlayer { m_pBoard->GetPlayerId(coordPos) };
			if (idPlayer == m_idPlayer)
				blockNeighbours(coordPos);
			else if (idPlayer != NO_PLAYER)
				blockPosition(coordPos);
		}
	);
}

void Player::UndoMove()
{
	m_bFinished = false;   
	m_iResult   = 0;
	++m_remainingPieces;
	if (m_remainingPieces == NR_OF_PIECE_TYPES)
		m_bFirstMove = true;
	RecalcListOfContactPnts();
	recalcMapOfValidCells();
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
