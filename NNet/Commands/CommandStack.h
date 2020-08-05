// CommandStack.h
//
// NNetModel

#pragma once

#include <vector>
#include <typeinfo>
#include "BoolOp.h"
#include "Observable.h"
#include "Command.h"
#include "MoveCommand.h"
#include "NNetModel.h"

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

    void NewCommand( Command * pCmd )
    {
        if ( m_bCombineMoveCmds && ! UndoStackEmpty() )
        {
            Command * pLastCommand { m_CommandStack[m_iIndex-1] };
            if ( pCmd->IsMoveCommand() && (typeid(*pCmd) == typeid(*pLastCommand)) )
            {
                MoveCommand * const pThis { static_cast<MoveCommand *>(pCmd) };
                MoveCommand * const pLast { static_cast<MoveCommand *>(pLastCommand) };
                if ( pLast->GetMovedShape() == pThis->GetMovedShape() )
                {
                    pLast->AddDelta( pThis );
                    pCmd->Do( m_pModel );
                    m_pModel->StaticModelChanged( );
                    return;
                }
            }
        }

        clearRedoStack( );
        m_CommandStack.push_back( pCmd );

#ifdef _DEBUG
        NNetModel const * pModelSave1 { new NNetModel( * m_pModel ) };
#endif
        pCmd->Do( m_pModel );
        m_iIndex = m_CommandStack.size();
        m_pModel->StaticModelChanged( );
        m_pObservable->NotifyAll( true );

#ifdef _DEBUG
        NNetModel const * pModelSave2 { new NNetModel( * m_pModel ) };
        UndoCommand();
        assert( m_pModel->IsEqual( * pModelSave1 ) );
        RedoCommand();
        assert( m_pModel->IsEqual( * pModelSave2 ) );
#endif
    }

    bool UndoCommand( )
    {
        if ( m_iIndex == 0 ) // stack is empty, nothing to undo
            return false;
        m_CommandStack[--m_iIndex]->Undo( m_pModel );
        m_pModel->StaticModelChanged( );
        m_pObservable->NotifyAll( true );
        return true;
    }

    bool RedoCommand( )
    {
        if ( m_iIndex == m_CommandStack.size() ) // top of stack, nothing to redo
            return false;
        m_CommandStack[m_iIndex++]->Do( m_pModel );
        m_pModel->StaticModelChanged( );
        m_pObservable->NotifyAll( true );
        return true;
    }

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
    bool              m_bCombineMoveCmds{ true };
    Observable      * m_pObservable     { nullptr };

    void clearRedoStack( )
    {
        for ( auto i = m_CommandStack.size(); i > m_iIndex; )
        {
            delete m_CommandStack[--i];
            m_CommandStack.pop_back();
        }
    }
};

