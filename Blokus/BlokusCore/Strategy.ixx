// Strategy.ixx
//
// BlokusCore

export module BlokusCore:Strategy;

import :Move;
import :RuleServerInterface;

export class Strategy
{
public:

    virtual Move SelectMove(RuleServerInterface const&) = 0;
};