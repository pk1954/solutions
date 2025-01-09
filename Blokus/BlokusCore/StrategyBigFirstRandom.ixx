//
// BlokusCore

export module BlokusCore:StrategyBigFirstRandom;

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
using std::ranges::find_if;
using std::ranges::subrange;

export class StrategyBigFirstRandom : public Strategy
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
            subrange<MoveIter> const sr    { moves.GetMovesWithMaxNrOfCells() };
            unsigned int       const uiMax { Cast2UnsignedInt(sr.size()) - 1 };
            unsigned int       const uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
            return moves[uiSel];
        }
    }

private:

    wstring const NAME { L"BigFirstRandom" };

    mutable Random m_random;
};