// CommandStack.h
//
// NNetModel

#pragma once

#include <vector>
#include <utility>
#include <typeinfo>
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

    void Initialize( NNetModel * const pModel, Observable * const pObservable )
    {
        m_pModel = pModel;
        m_pObservable = pObservable;
    }

    bool UndoStackEmpty() const 
    { 
        return m_iIndex == 0; 
    }

    bool RedoStackEmpty() const 
    { 
        return m_iIndex == m_CommandStack.size(); 
    }

    void NewCommand( Command * );
    bool UndoCommand( );
    bool RedoCommand( );

    bool SetCombineMoveCmdsFlag( tBoolOp const op )
    {
        bool bRet { m_bCombineMoveCmds };
        ApplyOp( m_bCombineMoveCmds, op );
        return bRet;
    }

private:

    vector<Command *> m_CommandStack    { };
    size_t            m_iIndex          { 0 }; // index after last valid m_CommandStack index. 0 means stack is empty.
    NNetModel       * m_pModel          { nullptr };
    Observable      * m_pObservable     { nullptr };
    bool              m_bCombineMoveCmds{ true };
    bool              m_bBracketIsOpen  { false };

    void clearRedoStack( )
    {
        for ( auto i = m_CommandStack.size(); i > m_iIndex; )
        {
            delete m_CommandStack[--i];
            m_CommandStack.pop_back();
        }
    }

    void push( Command * const pCmd )
    {
        m_CommandStack.push_back( pCmd );
        ++m_iIndex;
    }
};

