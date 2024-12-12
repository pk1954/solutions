// MatchReaderInterface.cpp
//
// BlokusCore

module BlokusCore:MatchReaderInterface;

using std::vector;

Piece const &MatchReaderInterface::GetPieceC(BlokusMove const move) const
{
	Player const &player { GetPlayerC(move.GetPlayerId()) };
	return player.GetPieceC(move.GetPieceTypeId());
}

PieceType const &MatchReaderInterface::GetPieceTypeC(BlokusMove const move) const
{
	Piece     const &piece     { GetPieceC(move) };
	PieceType const &pieceType { piece.GetPieceTypeC() };
	return pieceType;
}

Degrees MatchReaderInterface::GetRotation(BlokusMove const move) const
{
	Piece     const &piece     { GetPieceC(move) };
	PieceType const &pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
	Shape     const &shape     { pieceType.GetShapeC(move.GetShapeId()) };
	return shape.GetRotation();
}

void MatchReaderInterface::DrawSetPieces(DrawContext &context) const
{
    Apply2AllBoardCells
    (
        [this, &context](CoordPos const& pos)
        {
		    if (PlayerId const idPlayer { m_pMatch->m_board.GetPlayerId(pos) }; idPlayer != NO_PLAYER)
                GetPlayerC(idPlayer).DrawCell(context, pos);
        }
    );
}

void MatchReaderInterface::DrawMovePiece
(
    DrawContext     &context,
    BlokusMove const move
) const
{
	MicroMeterPnt const  umPosTarget { Convert2fCoord(move.GetCoordPos()) };
	Color         const  color       { IsValidPosition(move) ? ActiveColor() : COL_BLACK};
    PieceType     const &pieceType   { move.GetPieceTypeC() };
	pieceType.Draw(context, move.GetShapeId(), umPosTarget, color * 0.5f, false);
}

void MatchReaderInterface::DrawMovePiece
(
    DrawContext         &context,
    BlokusMove    const  move,
    MicroMeterPnt const &umPos
) const
{
	Color         const  color       { ActiveColor()};
    PieceType     const &pieceType   { move.GetPieceTypeC() };
	pieceType.Draw(context, move.GetShapeId(), umPos, color, false);
}

PlayerId MatchReaderInterface::WinnerId() const
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

bool MatchReaderInterface::HasContact(BlokusMove const move) const
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

bool MatchReaderInterface::IsValidPosition(BlokusMove const move) const
{
    return m_pMatch->AllShapeCellsUnblocked(move) && HasContact(move);
}
