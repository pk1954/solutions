// Strategy.ixx
//
// BlokusCore

export module BlokusCore:Strategy;

import std;
import :BlokusMove;
import :RuleServerInterface;

using std::wstring;

export class Strategy
{
public:

    virtual BlokusMove     SelectMove(RuleServerInterface const&) = 0;
    virtual wstring const& GetName() const                        = 0;
    virtual bool           IsHuman() const { return false; }
};