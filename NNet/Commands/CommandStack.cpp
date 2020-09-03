// CommandStack.cpp
//
// Commands

#include "stdafx.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"

class OpenBracket  : public Command { public: virtual void Do( NNetModelWriterInterface * const pModel ) { } };
class CloseBracket : public Command { public: virtual void Do( NNetModelWriterInterface * const pModel ) { } };

bool isOpenBracketCmd( Command const & cmd )
{
    return typeid(cmd) == typeid(OpenBracket);
}

bool isCloseBracketCmd( Command const & cmd )
{
    return typeid(cmd) == typeid(CloseBracket);
}

bool canBeCombined( Command * pA, Command * pB )
{
    return (typeid(* pA) == typeid(* pB)) &&
           ( 
               static_cast<MoveCommand *>(pA)->GetMovedShape() == 
               static_cast<MoveCommand *>(pB)->GetMovedShape() 
           );
}

void CommandStack::Initialize
( 
    NNetModelWriterInterface * const pModel, 
    Observable               * const pObservable 
)
{
    m_pModelInterFace = pModel;
    m_pObservable = pObservable;
}

bool CommandStack::isBracketCmd( )
{
    Command const & cmd { getCurrentCmd() };
    return isOpenBracketCmd( cmd ) || isCloseBracketCmd( cmd );
}

void CommandStack::Clear( )
{
    m_iIndex = 0;
    clearRedoStack();
}

void CommandStack::clearRedoStack( )
{
    for ( auto i = m_CommandStack.size(); i > m_iIndex; )
    {
        delete m_CommandStack[--i];
        m_CommandStack.pop_back();
    }
}

void CommandStack::push( Command * const pCmd )
{
    m_CommandStack.push_back( pCmd );
    ++m_iIndex;
}

Command & CommandStack::getCurrentCmd( )
{
    if ( (m_iIndex < 0) || ( m_iIndex >= m_CommandStack.size() ) )
        throw CmdStackException();
    return * m_CommandStack[ m_iIndex ];
}

void CommandStack::set2OlderCmd( )
{
    if ( UndoStackEmpty() ) 
        throw CmdStackException();
    --m_iIndex;
}

Command & CommandStack::getOlderCmd( )
{
    set2OlderCmd( );
    return * m_CommandStack[m_iIndex];
}

void CommandStack::set2YoungerCmd( )
{
    if ( RedoStackEmpty() ) 
        throw CmdStackException();
    ++m_iIndex;
}

Command & CommandStack::getYoungerCmd( )
{
    set2YoungerCmd( );
    return * m_CommandStack[m_iIndex];
}

void CommandStack::StartSeries( )
{
    push( new OpenBracket( ) ); 
}

void CommandStack::StopSeries( )
{
    push( new CloseBracket( ) ); 
}

void CommandStack::NewCommand( Command * pCmd )
{
#ifdef _DEBUG
    NNetModel const * pModelSave1 { new NNetModel( m_pModelInterFace->GetModel( ) ) };
#endif
    clearRedoStack( );
    if ( pCmd->IsMoveCommand( ) )
    {
        if ( 
              (m_iIndex > 2) && 
              isCloseBracketCmd( * m_CommandStack[m_iIndex-1] ) && 
              canBeCombined( pCmd, m_CommandStack[m_iIndex-2] )
           )
        {
            push( m_CommandStack[m_iIndex-1] );    // copy close bracket to new slot
            m_CommandStack[m_iIndex-2] = pCmd;     // and place pCmd before close bracket
        }
        else 
        {
            StartSeries( ); 
            push( pCmd );
            StopSeries( ); 
        }
    }
    else
    {
        push( pCmd );
    }

    pCmd->Do( m_pModelInterFace );
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );

#ifdef _DEBUG
    NNetModel const * pModelSave2 { new NNetModel( m_pModelInterFace->GetModel( ) ) };
    UndoCommand();
    assert( m_pModelInterFace->IsEqual( * pModelSave1 ) );
    RedoCommand();
    assert( m_pModelInterFace->IsEqual( * pModelSave2 ) );
#endif
}

bool CommandStack::UndoCommand( )
{
    try
    {
        if ( m_bCombineCmds )
        {
            if ( isCloseBracketCmd( getOlderCmd() ) )
                while ( ! isOpenBracketCmd( getOlderCmd() ) )
                    getCurrentCmd().Undo( m_pModelInterFace );
            else
                getCurrentCmd().Undo( m_pModelInterFace );
        }
        else
        {
            do
                set2OlderCmd( );
            while ( isBracketCmd( ) ); // skip bracket commands
            getCurrentCmd().Undo( m_pModelInterFace );
        }
    }
    catch ( CmdStackException e )
    {
        return false;
    }
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}

bool CommandStack::RedoCommand( )
{
    try
    {
        if ( m_bCombineCmds ) 
        {
            if ( isOpenBracketCmd( getCurrentCmd() ) )
                while ( ! isCloseBracketCmd( getYoungerCmd() ) )
                    getCurrentCmd().Do( m_pModelInterFace );
            else
                getCurrentCmd().Do( m_pModelInterFace );
        }
        else
        {
            while ( isBracketCmd( ) ) // skip bracket commands
                set2YoungerCmd();
            getCurrentCmd().Do( m_pModelInterFace );
        }
    }
    catch ( CmdStackException e )
    {
        return false;
    }
    set2YoungerCmd();
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}
