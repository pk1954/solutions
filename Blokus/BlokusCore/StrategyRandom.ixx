// StrategyRandom.ixx
//
// BlokusCore

export module BlokusCore:StrategyRandom;

import std;
import Random;
import :Move;
import :Board;
import :Strategy;
import :RuleServerInterface;

using std::vector;
using std::wstring;

export class StrategyRandom : public Strategy
{
public:

    wstring const& GetName() const final { return NAME; }

    Move SelectMove(RuleServerInterface const &rs) final
    {
        Move move;  // initialized to everything Undefined
        vector<Move> const& moves { rs.GetListOfValidMoves() };
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