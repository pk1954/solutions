// StrategyRandom.ixx
//
// BlokusCore

export module BlokusCore:StrategyRandom;

import std;
import Random;
import :Move;
import :Board;
import :Strategy;

using std::vector;

export class StrategyRandom : public Strategy
{
public:

    Move const& SelectMove(vector<Move> const& moves, Board const& board) final
    {
       unsigned int uiMax { Cast2UnsignedInt(moves.size()) - 1 };
       unsigned int uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
       return moves[uiSel];
    }

private:
    Random m_random;
};