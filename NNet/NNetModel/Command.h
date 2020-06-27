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
        for ( auto it = m_iter; it != m_CommandStack.end(); ++it )
            delete * it;
        m_CommandStack.erase( m_iter, m_CommandStack.end() );
        m_CommandStack.push_back( pCmd );
        pCmd->Do( m_pModel );
        m_iter = m_CommandStack.end();
    }

    bool UndoCommand( )
    {
        if ( m_iter == m_CommandStack.begin() )
            return false;
        (*--m_iter)->Undo( m_pModel );
        return true;
    }

    bool RedoCommand( )
    {
        if ( m_iter == m_CommandStack.end() )
            return false;
        (*m_iter++)->Do( m_pModel );
        return true;
    }

private:

    using CStack = vector<Command *>;

    CStack            m_CommandStack { };
    CStack::iterator  m_iter         { m_CommandStack.end() };
    NNetModel       * m_pModel       { nullptr };
};

