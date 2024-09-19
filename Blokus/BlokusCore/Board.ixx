// Board.ixx
//
// BlokusCore

export module BlokusCore:Board;

import :Game;

using std::array;

import std;

export class Board
{
public:

    bool IsOnBoard(CoordPos const& pos) const
    {
        return IsInRange(pos.GetX(), 0_COORD, Game::MAX_COORD) && 
               IsInRange(pos.GetY(), 0_COORD, Game::MAX_COORD);
    }

    PlayerId GetPlayerId(CoordPos const& pos) const
    {
        return m_board[pos.GetYvalue()][pos.GetXvalue()];
    }

    bool IsFreeCell(CoordPos const& pos) const
    {
        return IsOnBoard(pos) && (GetPlayerId(pos) == NO_PLAYER);
    }

private:

    array<array<PlayerId, Components::BOARD_SIZE>, Components::BOARD_SIZE> m_board;
};