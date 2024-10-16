// StrategyTakeFirst.ixx
//
// BlokusCore

export module BlokusCore:StrategyTakeFirst;

import std;
import :BlokusMove;
import :Board;
import :Strategy;
import :RuleServerInterface;

using std::vector;
using std::wstring;

export class StrategyTakeFirst : public Strategy
{
public:

    wstring const& GetName() const final { return NAME; }


    BlokusMove SelectMove(RuleServerInterface const &rs) final
    {
        BlokusMove move;  // initialized to everything Undefined
        vector<BlokusMove> const& moves { rs.GetListOfValidMoves() };
        if (!moves.empty())
            move = moves[0];
        return move;
    }

private:

    wstring const NAME { L"TakeFirst" };
};