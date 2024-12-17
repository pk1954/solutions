// RuleServerInterface.ixx
//
// BlokusCore

export module BlokusCore:RuleServerInterface;

import std;
import :BlokusMove;
import :Board;

using std::vector;

export class RuleServerInterface
{
public:
    virtual ListOfMoves const &GetListOfValidMoves() const = 0;
    virtual Board       const &GetBoard()            const = 0;
};