// StrategyRandom.ixx
//
// BlokusCore

export module BlokusCore:StrategyRandom;

import std;
import Random;
import SaveCast;
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

    BlokusMove SelectMove(RuleServerInterface const &rsi) const final
    {
        ListOfMoves const &moves { rsi.GetListOfValidMoves() };
        if (!moves.empty())
        {
            unsigned int uiMax { Cast2UnsignedInt(moves.size()) - 1 };
            unsigned int uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
            return moves[uiSel];
        }
        return BlokusMove(); // initialized to everything Undefined
    }

private:

    wstring const NAME { L"Random" };

    mutable Random m_random;
};