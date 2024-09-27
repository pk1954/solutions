// StrategyTakeFirst.ixx
//
// BlokusCore

export module BlokusCore:StrategyTakeFirst;

import std;
import :Move;
import :Board;
import :Strategy;
import :RuleServerInterface;

using std::vector;

export class StrategyTakeFirst : public Strategy
{
public:

    Move SelectMove(RuleServerInterface const &rs) final
    {
        Move move;  // initialized to everything Undefined
        vector<Move> const& moves { rs.GetListOfValidMoves() };
        if (!moves.empty())
            move = moves[0];
        return move;
    }
};