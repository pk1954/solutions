// StrategyTakeFirst.ixx
//
// BlokusCore

export module BlokusCore:StrategyTakeFirst;

import std;
import :Move;
import :Board;
import :Strategy;

using std::vector;

export class StrategyTakeFirst : public Strategy
{
public:

    Move const& SelectMove(vector<Move> const& moves, Board const& board) final
    {
       return moves[0];
    }
};