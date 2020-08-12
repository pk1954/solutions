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
#include "NNetModel.h"

using std::swap;
using std::vector;

class CommandStack
{
public:

    void Initialize( NNetModel * const, Observable * const );

    bool UndoStackEmpty() const { return m_iIndex == 0; }
    bool RedoStackEmpty() const { return m_iIndex == m_CommandStack.size(); }

    void SetCombineCmdsFlag( bool const op ) { m_bCombineCmds = op; }
    bool GetCombineCmdsFlag( ) const { return m_bCombineCmds;  }

    void StartSeries();
    void StopSeries();

    void NewCommand( Command * );
    bool UndoCommand( );
    bool RedoCommand( );

    void Clear( );

private:

    vector<Command *> m_CommandStack   { };
    size_t            m_iIndex         { 0 };
    NNetModel       * m_pModel         { nullptr };
    Observable      * m_pObservable    { nullptr };
    bool              m_bCombineCmds   { true };

    struct CmdStackException: public std::exception { };

    void      clearRedoStack( );
    void      push( Command * const );
    Command & getCurrentCmd( );
    void      set2OlderCmd( );
    Command & getOlderCmd( );
    void      set2YoungerCmd( );
    Command & getYoungerCmd( );
    bool      isBracketCmd( );
};
