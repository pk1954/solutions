// RuleServerInterface.ixx
//
// BlokusCore

export module BlokusCore:RuleServerInterface;

import std;
import :Move;
import :Board;

using std::vector;

export class RuleServerInterface
{
public:
    virtual vector<Move> const &GetListOfValidMoves() const = 0;
    virtual Board        const &GetBoard()            const = 0;
};