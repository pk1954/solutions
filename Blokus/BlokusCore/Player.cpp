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
using std::ranges::subrange;
using std::setw;

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

void Player::dumpContactPnts() const
{
	wcout << L"** List of contact points" << endl;
	Apply2AllContactPntsC([](CoordPos const& pos) { wcout << pos << endl; } );
}

void Player::dumpListOfValidMoves() const
{
	wcout << L"** List of valid moves" << endl;
	if (m_listOfValidMoves.Empty())
		wcout << L"  -- no valid moves --" << endl;
	else 
		m_listOfValidMoves.Apply2AllC([](BlokusMove const& move){ wcout << move << endl; });
}

void Player::dumpMapOfMoveablePieces() const
{
	wcout << L"** List of movable pieces:";
	for (int i = 0; i < NR_OF_PIECE_TYPES; ++i)
		if (m_mapOfMoveablePieces[i])
			wcout << setw(2) << i << SPACE;
	wcout << endl;
}

void Player::Dump() const
{
	wcout << L"*** Player " << m_idPlayer << endl;
	wcout << L"Tables are " << (m_bTablesValid ? L"valid" : L"invalid") << endl;
	if (m_bTablesValid)
	{
		m_mapOfValidCells.Dump();
		dumpContactPnts();
		dumpListOfValidMoves();
		dumpMapOfMoveablePieces();
	}
}

void Player::Reset()
{
    PieceTypeId id { 0 };
    Apply2AllPieces([&id](Piece& p){ p.Initialize(id++); });
    m_bFirstMove   = true;
	m_bTablesValid = false;
	m_idPieceTypeLastMove = UndefinedPieceTypeId;
	m_mapOfValidCells.Reset();
	Prepare();
}

void Player::Notify(bool const bImmediately)
{
	m_bTablesValid = false;
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
				if (!IsMoveable(piece.GetPieceTypeId()))
					piece.Draw(context, posDir, Color(0.0f, 0.0f, 0.0f, 0.3f), false);
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

void Player::recalcListOfContactPnts() const
{
    m_listOfContactPnts.clear();
    if (IsFirstMove())
    {
        m_listOfContactPnts.push_back(m_pPlayerType->m_startPoint);
    }
    else
    {
        Apply2AllBoardCells
        (
            [this](CoordPos const& pos)
            {
                if (m_pBoard->IsContactPnt(pos, m_idPlayer))
                {
                    m_listOfContactPnts.push_back(pos);
                }
            }
        );
    }
}

bool Player::AnyShapeCellsBlocked(BlokusMove const move) const
{
    return AnyShapeCellsBlocked(move.GetShapeC(), move.GetCoordPos());
}

bool Player::AnyShapeCellsBlocked
(
	Shape    const &shape,
	CoordPos const &posShape
) const
{
    return shape.IsTrue4AnyShapeCell
    (
        [this, &posShape](ShapeCoordPos const &cellPos)
        {
            return IsBlocked(posShape + cellPos);
        }
    );
}

void Player::recalcMapOfValidCells() const
{
	Apply2AllBoardCells
	(
		[this](CoordPos const& coordPos)
		{
			PlayerId idPlayer { m_pBoard->GetPlayerId(coordPos) };
			if (idPlayer != NO_PLAYER)
				m_mapOfValidCells.BlockPosition(coordPos);
			if (idPlayer == m_idPlayer)
				m_mapOfValidCells.BlockNeighbours(coordPos);
		}
	);
}

void Player::testPosition
(
    BlokusMove          &move, 
    ShapeCoordPos const &posCorner
) const
{ 
    Apply2AllContactPntsC
    (
        [this, &move, &posCorner](CoordPos const& posContact)
        {
            move.SetCoordPos(posContact - posCorner);
            if (AnyShapeCellsBlocked(move))
                return;
            m_listOfValidMoves.Add(move);
			m_mapOfMoveablePieces[move.GetPieceTypeId().GetValue()] = true;
        }
	);
}

void Player::testShape(BlokusMove &move) const
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
) const
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

void Player::calcListOfValidMoves() const
{
    BlokusMove move;
    move.SetPlayerId(m_idPlayer);
	m_listOfValidMoves.Clear();
    Apply2AvailablePiecesC
    (
        [this, &move](Piece const& piece)
        {
            testPiece(move, piece);
        }
    );
	if (m_listOfValidMoves.Empty())
		m_iResult = calcResult();
	m_listOfValidMoves.Apply2AllC([this](BlokusMove const move){ Assert(!AnyShapeCellsBlocked(move)); });
}

void Player::Prepare() const
{
	if (!m_bTablesValid)
	{
		m_mapOfMoveablePieces.fill(false);
		recalcListOfContactPnts();
		recalcMapOfValidCells();
		calcListOfValidMoves();
		m_bTablesValid = true;
	}
}

ListOfMoves const& Player::GetListOfValidMoves() const
{
	Assert(m_bTablesValid);
    subrange<MoveIter> it = m_listOfValidMoves.GetMoves(PieceTypeId(8)); 
	return m_listOfValidMoves;
}

BlokusMove Player::SelectMove(RuleServerInterface const &rsi) const
{
	//m_timer.BeforeAction();
	BlokusMove moveSelected { m_pStrategy->SelectMove(m_idPlayer, rsi) };
 	//m_timer.AfterAction();
    return moveSelected;
}

int Player::calcResult() const
{
	int iResult = 0;
	Apply2AvailablePiecesC
	(
		[this, &iResult](Piece const& piece)
		{
			iResult -= piece.GetPieceTypeC().NrOfCells();
		}
	);
	if (iResult == 0)    // all pieces set
	{
		iResult += 15;
		if (Components::GetPieceTypeC(m_idPieceTypeLastMove).NrOfCells() == 1)
			iResult += 5;
	}
	return iResult;
}

void Player::DoMove(BlokusMove &move)
{
    Assert(move.IsDefined());
	m_bFirstMove = false;
	m_idPieceTypeLastMove = move.GetPieceTypeId();
	Prepare();
}
