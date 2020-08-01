// Command.h
//
// NNetModel

#pragma once

#include <vector>
#include <optional>
#include "NNetModel.h"

using std::vector;

class Command
{
public:
    virtual void Do  ( NNetModel * const ) = 0;
    virtual void Undo( NNetModel * const pModel ) { Do( pModel ); };
};

class CommandStack
{
public:

    void Inizialize( NNetModel * const pModel )
    {
        m_pModel = pModel;
    }

    void NewCommand( Command * pCmd )
    {
        clearRedoStack( );
        m_CommandStack.push_back( pCmd );

#ifdef _DEBUG
        NNetModel const * pModelSave1 { new NNetModel( * m_pModel ) };
#endif
        pCmd->Do( m_pModel );
        m_iIndex = m_CommandStack.size();
        m_pModel->StaticModelChanged( );

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
        return true;
    }

    bool RedoCommand( )
    {
        if ( m_iIndex == m_CommandStack.size() ) // top of stack, nothing to redo
            return false;
        m_CommandStack[m_iIndex++]->Do( m_pModel );
        m_pModel->StaticModelChanged( );
        return true;
    }

private:

    using CStack = vector<Command *>;

    CStack      m_CommandStack { };
    size_t      m_iIndex       { 0 }; // index after last valid m_CommandStack index. 0 means stack is empty.
    NNetModel * m_pModel       { nullptr };

    void clearRedoStack( )
    {
        for ( auto i = m_CommandStack.size(); i > m_iIndex; )
        {
            delete m_CommandStack[--i];
            m_CommandStack.pop_back();
        }
    }
};

