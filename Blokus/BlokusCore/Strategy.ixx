// Strategy.ixx
//
// BlokusCore

export module BlokusCore:Strategy;

import std;
import :Move;
import :RuleServerInterface;

using std::wstring;

export class Strategy
{
public:

    virtual Move           SelectMove(RuleServerInterface const&) = 0;
    virtual wstring const& GetName() const                        = 0;
};