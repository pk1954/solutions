// RuleServerInterface.ixx
//
// BlokusCore

export module BlokusCore:RuleServerInterface;

import std;
import :BlokusMove;
import :Board;
import :PlayerId;
import :ListOfMoves;

export class RuleServerInterface
{
public:
    virtual ListOfMoves const &GetListOfValidMoves(PlayerId const) const = 0;
    virtual Board       const &GetBoard()                          const = 0;
};