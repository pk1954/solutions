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
        Shape const& shape { GetShapeC(move, players) };
        return shape.Apply2AllShapeCellsB
        (
            [this, &move](ShapeCoordPos const &shapePos)
            {
                CoordPos const coordPos { move.m_boardPos + shapePos };
                return IsFreeCell(move.m_boardPos + shapePos);
            }
        );
    }

    void PerformMove
    (
        Move    const& move,
        Players const& players
    )
    {
        GetShapeC(move, players).Apply2AllShapeCells
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

    array<array<PlayerId, BOARD_SIZE>, BOARD_SIZE> m_cells;
};