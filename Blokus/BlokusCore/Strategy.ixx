// Strategy.ixx
//
// BlokusCore

export module BlokusCore:Strategy;

import std;
import :Move;
import :Board;

using std::vector;

export class Strategy
{
public:

    virtual Move const& SelectMove(vector<Move> const&, Board const&) = 0;
};