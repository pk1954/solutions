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
    Board();

    void Reset();
    void SetPlayerId (CoordPos const&, PlayerId const);
    void PerformMove(BlokusMove const&);

    PlayerId GetPlayerId(CoordPos const&)              const;
    bool IsFreeCell  (CoordPos const&)                 const;
    bool IsContactPnt(CoordPos const&, PlayerId const) const;

private:
    bool hasPlayerId    (CoordPos const&, PlayerId const) const;
    bool hasDiagContact (CoordPos const&, PlayerId const) const;
    bool hasOrthoContact(CoordPos const&, PlayerId const) const;

    array<array<PlayerId, BOARD_SIZE>, BOARD_SIZE> m_cells;
};