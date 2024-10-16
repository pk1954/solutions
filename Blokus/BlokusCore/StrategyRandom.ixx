// StrategyRandom.ixx
//
// BlokusCore

export module BlokusCore:StrategyRandom;

import std;
import Random;
import :Board;
import :Strategy;
import :BlokusMove;
import :RuleServerInterface;

using std::vector;
using std::wstring;

export class StrategyRandom : public Strategy
{
public:

    wstring const& GetName() const final { return NAME; }

    BlokusMove SelectMove(RuleServerInterface const &rs) final
    {
        BlokusMove move;  // initialized to everything Undefined
        vector<BlokusMove> const& moves { rs.GetListOfValidMoves() };
        if (!moves.empty())
        {
            unsigned int uiMax { Cast2UnsignedInt(moves.size()) - 1 };
            unsigned int uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
            return moves[uiSel];
        }
        return move;
    }

private:

    wstring const NAME { L"Random" };

    Random m_random;
};