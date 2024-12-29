// MatchReaderInterface.cpp
//
// BlokusCore

module BlokusCore:MatchReaderInterface;

using std::move;
using std::vector;
using std::to_wstring;

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
    Apply2AllPlayersC
    (
        [&context](Player const &player)
        { 
            player.DrawSetPieces(context); 
        }
    );
}

void MatchReaderInterface::DrawMovePiece
(
    DrawContext     &context,
    BlokusMove const move,
    Color      const color
) const
{
	MicroMeterPnt const umPosTarget { Convert2fCoord(move.GetCoordPos()) };
	GetPieceC(move).Draw
    (
        context, 
        umPosTarget, 
        color * 0.5f, 
        false, 
        move.GetShapeId()
    );
}

void MatchReaderInterface::DrawMovePiece
(
    DrawContext         &context,
    BlokusMove    const  move,
    Color         const  color,
    MicroMeterPnt const &umPos
) const
{
	GetPieceC(move).Draw
    (
        context, 
        umPos, 
        color, 
        false, 
        move.GetShapeId()
    );
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
            return GetPlayerC(move.GetPlayerId()).IsTrue4AnyContactPnt
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
    return HasContact(move) && !m_pMatch->AnyShapeCellsBlocked(move);
}
