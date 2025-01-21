// StrategyTakeFirst.ixx
//
// BlokusCore

export module BlokusCore:StrategyTakeFirst;

import std;
import :BlokusMove;
import :Board;
import :PlayerId;
import :Strategy;
import :RuleServerInterface;

using std::vector;
using std::wstring;

export class StrategyTakeFirst : public Strategy
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
            return moves[0];
    }

    inline static wstring const NAME { L"TakeFirst" };

private:
};