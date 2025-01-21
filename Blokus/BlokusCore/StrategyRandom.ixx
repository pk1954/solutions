// StrategyRandom.ixx
//
// BlokusCore

export module BlokusCore:StrategyRandom;

import std;
import Random;
import SaveCast;
import :Board;
import :PlayerId;
import :Strategy;
import :BlokusMove;
import :RuleServerInterface;

using std::vector;
using std::wstring;

export class StrategyRandom : public Strategy
{
public:

    wstring const& GetName() const final { return NAME; }

    BlokusMove SelectMove
    (
        PlayerId            const  idPlayer,
        RuleServerInterface const &rsi
    ) const final
    {
        ListOfMoves const &moves { rsi.GetListOfValidMoves(idPlayer) };
        if (moves.Empty())
            return BlokusMove(); // initialized to everything Undefined
        else
        {
            unsigned int uiMax { Cast2UnsignedInt(moves.Size()) - 1 };
            unsigned int uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
            return moves[uiSel];
        }
    }

    inline static wstring const NAME { L"Random" };

private:

    mutable Random m_random;
};