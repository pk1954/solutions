// Board.ixx
//
// BlokusCore

export module BlokusCore:Board;

import std;
import :Player;
import :Players;
import :Components;

using std::array;
using std::vector;

export class Board
{
public:
    Board()
    {
        for (int i = 0; i < BOARD_SIZE; ++i) 
        for (int j = 0; j < BOARD_SIZE; ++j) 
            m_cells[i][j] = NO_PLAYER;
    }

    PlayerId GetPlayerId(CoordPos const& pos) const
    {
        return m_cells[pos.GetYvalue()][pos.GetXvalue()];
    }

    void SetPlayerId(CoordPos const& pos, PlayerId const id)
    {
        m_cells[pos.GetYvalue()][pos.GetXvalue()] = id;
    }

    bool IsFreeCell(CoordPos const& pos) const
    {
        return IsOnBoard(pos) && (GetPlayerId(pos) == NO_PLAYER);
    }

    Shape const& GetShapeC
    (
        Move    const& move,
        Players const& players
    )
    {
        Player    const& player    { players    .GetPlayerC   (move.m_idPlayer) };
        PieceType const& pieceType { Components::GetPieceTypeC(move.m_idPieceType) };
        Shape     const& shape     { pieceType  .GetShapeC    (move.m_idShape) };
        return shape;
    }

    bool IsValidMove
    (
        Move    const& move,
        Players const& players
    )
    {
        Player    const& player    { players    .GetPlayerC   (move.m_idPlayer) };
        PieceType const& pieceType { Components::GetPieceTypeC(move.m_idPieceType) };
        Shape     const& shape     { pieceType  .GetShapeC    (move.m_idShape) };
        return shape.Apply2AllShapeCellsB
        (
            [this, &move, &player](ShapeCoordPos const &shapePos)
            {
                CoordPos const coordPos { move.m_boardPos + shapePos };
                return IsFreeCell(coordPos) && player.IsValidPos(coordPos);
            }
        );
    }

    bool IsContactPnt(CoordPos const& pos, PlayerId const id)
    {
	    if (!IsFreeCell(pos))
            return false;
        if (!hasDiagContact(pos, id))
            return false;
        if (hasOrthoContact(pos, id))
            return false;
        return true;
    }

    void PerformMove
    (
        Move    const& move,
        Players const& players
    )
    {
        GetShapeC(move, players).Apply2AllShapeCellsC
        (
            [this, &move](ShapeCoordPos const &shapePos)
            {
                CoordPos const coordPos { move.m_boardPos + shapePos };
                Assert(IsOnBoard(coordPos));
                SetPlayerId(coordPos, move.m_idPlayer);
            }
        );
    }

private:
    // A cell in the environment of a cell is a contact point 
    // if at least one diagonal neighbour of the cell belongs to the same player
    // and no orthogonal (horz or vert) neighbour of the cell belongs to the same player

    bool hasPlayerId(CoordPos const& pos, PlayerId const id) const
    {
        return IsOnBoard(pos) && (GetPlayerId(pos) == id);
    }

    bool hasDiagContact(CoordPos const& pos, PlayerId const id) const
    {
	    return (hasPlayerId(NorthEastPos(pos), id)) || 
               (hasPlayerId(SouthEastPos(pos), id)) ||
		       (hasPlayerId(NorthWestPos(pos), id)) || 
               (hasPlayerId(SouthWestPos(pos), id));
    }

    bool hasOrthoContact(CoordPos const& pos, PlayerId const id) const
    {
        return (hasPlayerId(NorthPos(pos), id)) ||
               (hasPlayerId(EastPos (pos), id)) ||
		       (hasPlayerId(SouthPos(pos), id)) ||
               (hasPlayerId(WestPos (pos), id));
    }

    array<array<PlayerId, BOARD_SIZE>, BOARD_SIZE> m_cells;
};