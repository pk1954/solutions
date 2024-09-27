// Strategy.ixx
//
// BlokusCore

export module BlokusCore:Strategy;

import std;
import :Move;
import :Board;
import :RuleServerInterface;

using std::vector;

export class Strategy
{
public:

    virtual Move SelectMove(RuleServerInterface const&) = 0;
};