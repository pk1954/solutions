// Board.ixx
//
// BlokusCore

export module BlokusCore:Board;

import std;
import :Components;
import :BlokusUtilities;

using std::array;
using std::vector;

export class Board
{
public:
    Board()
    {
        Reset();
    }

    void Reset()
    {
        for (int i = 0; i < BOARD_SIZE; ++i) 
        for (int j = 0; j < BOARD_SIZE; ++j) 
            m_cells[i][j] = NO_PLAYER;
    }

    PlayerId GetPlayerId(CoordPos const& pos) const
    {
        return m_cells.at(pos.GetYvalue()).at(pos.GetXvalue());
    }

    void SetPlayerId(CoordPos const& pos, PlayerId const id)
    {
        m_cells.at(pos.GetYvalue()).at(pos.GetXvalue()) = id;
    }

    bool IsFreeCell(CoordPos const& pos) const
    {
        return IsOnBoard(pos) && (GetPlayerId(pos) == NO_PLAYER);
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

    void PerformMove(Move const& move)
    {
        GetShapeC(move).Apply2AllShapeCellsC
        (
            [this, &move](ShapeCoordPos const &shapePos)
            {
                CoordPos const coordPos { move.GetCoordPos() + shapePos };
                Assert(IsOnBoard(coordPos));
                SetPlayerId(coordPos, move.GetPlayerId());
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