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
        if (!moves.Empty())
            return moves[0];
        return BlokusMove(); // initialized to everything Undefined
    }

private:

    wstring const NAME { L"TakeFirst" };
};