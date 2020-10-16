// CommandStack.h
//
// NNetModel

#pragma once

#include <vector>
#include <utility>
#include <typeinfo>
#include <exception>
#include "BoolOp.h"
#include "Observable.h"
#include "Command.h"
#include "MoveCommand.h"

using std::swap;
using std::vector;
using std::unique_ptr;

class NNetModelWriterInterface;

class CommandStack
{
public:

    void Initialize( NNetModelWriterInterface * const, Observable * const );

    bool UndoStackEmpty() const { return m_iIndex == 0; }
    bool RedoStackEmpty() const { return m_iIndex == m_CommandStack.size(); }

    void SetCombineCmdsFlag( bool const op ) { m_bCombineCmds = op; }
    bool GetCombineCmdsFlag( ) const { return m_bCombineCmds;  }

    void StartSeries();
    void StopSeries();

    void NewCommand( unique_ptr<Command> );
    bool UndoCommand( );
    bool RedoCommand( );

    void Clear( );

private:

    vector<unique_ptr<Command>> m_CommandStack    { };
    size_t                      m_iIndex          { 0 };     // index into m_Commandstack
    bool                        m_bBracketOpen    { false }; // index in series?
    bool                        m_bCombineCmds    { false };
    NNetModelWriterInterface  * m_pModelInterFace { nullptr };
    Observable                * m_pObservable     { nullptr };

    struct CmdStackException: public std::exception { };

    Command & getCurrentCmd( );
    Command & getOlderCmd( );
    Command & getYoungerCmd( );

    void clearRedoStack( );
    void push( unique_ptr<Command> );
    void set2OlderCmd( );
    void set2YoungerCmd( );
    bool isBracketCmd( );
    bool isOpenBracketCmd( );
    bool isCloseBracketCmd( );
    void undoAndSetToOlder();
    void doAndSetToYounger();
};
