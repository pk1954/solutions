// CommandStack.cpp
//
// Commands

#include "stdafx.h"
#include "CommandStack.h"

class OpenBracket : public Command
{
public:
    virtual void Do( NNetModel * const pModel ) { }
};

bool isOpenBracketCmd( Command const * const pCmd )
{
    return typeid(* pCmd) == typeid(OpenBracket);
}

class CloseBracket : public Command
{
public:
    virtual void Do( NNetModel * const pModel ) { }
};

bool isCloseBracketCmd( Command const * const pCmd )
{
    return typeid(* pCmd) == typeid(CloseBracket);
}

bool isBracketCmd( Command const * const pCmd )
{
    return isOpenBracketCmd( pCmd ) || isCloseBracketCmd( pCmd );
}

bool canBeCombined( Command * pCmd1, Command * pCmd2 )
{
    return (typeid(* pCmd1) == typeid(* pCmd2)) &&
        ( static_cast<MoveCommand *>(pCmd1)->GetMovedShape() == static_cast<MoveCommand *>(pCmd2)->GetMovedShape() );
}

void CommandStack::NewCommand( Command * pCmd )
{
#ifdef _DEBUG
    NNetModel const * pModelSave1 { new NNetModel( * m_pModel ) };
#endif
    clearRedoStack( );
    if ( pCmd->IsMoveCommand( ) )
    {
        if ( 
            (m_iIndex > 2) && 
            isCloseBracketCmd( m_CommandStack[m_iIndex-1] ) && 
            canBeCombined( pCmd, m_CommandStack[m_iIndex-2] )
            )
        {
            push( m_CommandStack[m_iIndex-1] );    // copy close bracket to new slot
            m_CommandStack[m_iIndex-2] = pCmd;     // and place pCmd before close bracket
        }
        else 
        {
            push( new OpenBracket( ) ); 
            push( pCmd );
            push( new CloseBracket( ) ); 
        }
    }
    else
    {
        push( pCmd );
    }

    pCmd->Do( m_pModel );
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

bool CommandStack::UndoCommand( )
{
    if ( m_bCombineMoveCmds )
    {
        if (m_iIndex == 0)
            return false;
        --m_iIndex;
        if ( isCloseBracketCmd( m_CommandStack[m_iIndex] ) )
        {
            do 
                m_CommandStack[--m_iIndex]->Undo( m_pModel );
            while ( ! isOpenBracketCmd( m_CommandStack[m_iIndex] ) );    
        } 
        else
        {
            m_CommandStack[m_iIndex]->Undo( m_pModel );
        }
    }
    else
    {
        do
        {
            if (m_iIndex == 0)
                return false; 
        } while ( isBracketCmd( m_CommandStack[--m_iIndex] ) ); // skip bracket commands
        m_CommandStack[m_iIndex]->Undo( m_pModel );
    }
    m_pModel->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}

bool CommandStack::RedoCommand( )
{
    if ( m_bCombineMoveCmds ) 
    {
        if ( m_iIndex == m_CommandStack.size() ) // top of stack, nothing to redo
            return false;
        if ( isOpenBracketCmd( m_CommandStack[m_iIndex] ) )
        {
            do
                m_CommandStack[m_iIndex++]->Do( m_pModel );
            while ( ! isCloseBracketCmd( m_CommandStack[m_iIndex] ) );
        }
        else
        {
            m_CommandStack[m_iIndex]->Do( m_pModel );
        }
    }
    else
    {
        for (;;)
        {
            if ( m_iIndex == m_CommandStack.size() ) // top of stack, nothing to redo
                return false;
            if ( isBracketCmd( m_CommandStack[m_iIndex] ) ) // skip bracket commands
                ++m_iIndex;
            else
                break;
        }
        m_CommandStack[m_iIndex]->Do( m_pModel );
    }
    ++m_iIndex;
    m_pModel->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}
