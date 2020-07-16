// Command.h
//
// NNetModel

#pragma once

#include <vector>
#include <optional>

class NNetModel;

using std::vector;
using std::nullopt;

class Command
{
public:
    virtual void Do  ( NNetModel * ) = 0;
    virtual void Undo( NNetModel * ) = 0;
    virtual void Redo( NNetModel * pModel ) 
    { 
        Do( pModel ); 
    };
};

class CommandStack
{
public:

    void Inizialize( NNetModel* const pModel )
    {
        m_pModel = pModel;
    }

    void NewCommand( Command * pCmd )
    {
        for ( auto i = m_CommandStack.size(); i > m_iIndex; )
        {
            delete m_CommandStack[--i];
            m_CommandStack.pop_back();
        }
        m_CommandStack.push_back( pCmd );
        pCmd->Do( m_pModel );
        m_iIndex = m_CommandStack.size();
        m_pModel->StaticModelChanged( );
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
        if ( m_iIndex == m_CommandStack.size() ) // top of stack, nothing do redo
            return false;
        m_CommandStack[m_iIndex++]->Redo( m_pModel );
        m_pModel->StaticModelChanged( );
        return true;
    }

private:

    using CStack = vector<Command *>;

    CStack      m_CommandStack { };
    size_t      m_iIndex       { 0 }; // index after last valid m_CommandStack index. 0 means stack is empty.
    NNetModel * m_pModel       { nullptr };
};

