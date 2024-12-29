// StrategyHuman.ixx
//
// BlokusCore

export module BlokusCore:StrategyHuman;

import std;
import :Strategy;
import :BlokusMove;
import :RuleServerInterface;

using std::vector;
using std::wstring;

export class StrategyHuman : public Strategy
{
public:

    wstring const& GetName() const final { return NAME; }

    BlokusMove SelectMove
    (
        PlayerId            const  idPlayer, 
        RuleServerInterface const &rs
    ) const final
    {
        return BlokusMove();  // initialized to everything Undefined
    }

    bool IsHuman() const final { return true; }

private:

    wstring const NAME { L"Human" };
};
