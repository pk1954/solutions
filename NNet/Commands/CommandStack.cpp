// CommandStack.cpp
//
// Commands

#include "stdafx.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"

using std::wcout;
using std::endl;
using std::make_unique;

class OpenBracket  : public Command 
{ 
public: 
    virtual void Do( NNetModelWriterInterface & ) { } 
};
class CloseBracket : public Command 
{ 
public: 
    virtual void Do( NNetModelWriterInterface & ) { } 
};

bool CommandStack::isOpenBracketCmd( )
{
    return typeid(getCurrentCmd()) == typeid(OpenBracket);
}

bool CommandStack::isCloseBracketCmd( )
{
    return typeid(getCurrentCmd()) == typeid(CloseBracket);
}

bool canBeCombined( Command const & A, Command const & B )
{
    return A.IsMoveCommand( ) && (typeid(A) == typeid(B)) &&
           ( 
               static_cast<MoveCommand const &>(A).GetMovedShape() == 
               static_cast<MoveCommand const &>(B).GetMovedShape() 
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
    return isOpenBracketCmd() || isCloseBracketCmd();
}

void CommandStack::Clear( )
{
    m_iIndex = 0;
    clearRedoStack();
}

void CommandStack::clearRedoStack( )
{
    m_CommandStack.erase( m_CommandStack.begin() + m_iIndex, m_CommandStack.end() );
    //for ( auto i = m_CommandStack.size(); i > m_iIndex; -- i )
    //{
    //    m_CommandStack.pop_back();
    //}
    assert( RedoStackEmpty() );
}

void CommandStack::push( unique_ptr<Command> pCmd )
{
    m_CommandStack.push_back( move(pCmd) );
    ++m_iIndex;
}

Command & CommandStack::getCurrentCmd( )
{
    if ( (m_iIndex < 0) || (m_iIndex >= m_CommandStack.size()) )
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
    if ( UndoStackEmpty() ) 
        throw CmdStackException();
    return * m_CommandStack[m_iIndex-1];
}

void CommandStack::set2YoungerCmd( )
{
    if ( RedoStackEmpty() ) 
        throw CmdStackException();
    ++m_iIndex;
}

Command & CommandStack::getYoungerCmd( )
{
    if ( RedoStackEmpty() ) 
        throw CmdStackException();
    return * m_CommandStack[m_iIndex+1];
}

void CommandStack::StartSeries( )
{
    push( make_unique<OpenBracket>( OpenBracket( ) ) );
    m_bBracketOpen = true;
}

void CommandStack::StopSeries( )
{
    push( make_unique<CloseBracket>( CloseBracket( ) ) ); 
    m_bBracketOpen = false;
}

void CommandStack::undoAndSetToOlder( )
{
    wcout << L"CommandStack::undoAndSetToOlder " << L"index =" << m_iIndex << endl;
    getCurrentCmd().Undo( * m_pModelInterFace );
    set2OlderCmd();
}

void CommandStack::doAndSetToYounger( )
{
    wcout << L"CommandStack::doAndSetToYounger " << L"index =" << m_iIndex << endl;
    getCurrentCmd().Do( * m_pModelInterFace );
    set2YoungerCmd();
}

void CommandStack::PushCommand( unique_ptr<Command> pCmd )
{
#ifdef _DEBUG
    NNetModel const * pModelSave1 { new NNetModel( m_pModelInterFace->GetModel( ) ) };
    m_pModelInterFace->CheckModel();
    pModelSave1->CheckModel();
#endif
    pCmd->Do( * m_pModelInterFace );
    clearRedoStack( );
    if ( m_bBracketOpen && ! canBeCombined( * pCmd, getOlderCmd() ) )
        StopSeries( ); 
    if ( pCmd->IsMoveCommand( ) && ! m_bBracketOpen )
        StartSeries( ); 
    push( move(pCmd) );
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );

#ifdef _DEBUG
    NNetModel const * pModelSave2 { new NNetModel( m_pModelInterFace->GetModel( ) ) };
    pModelSave2->CheckModel();
    m_pModelInterFace->CheckModel();
    UndoCommand();
    m_pModelInterFace->CheckModel();
    assert( m_pModelInterFace->IsEqual( * pModelSave1 ) );
    m_pModelInterFace->CheckModel();
    RedoCommand();
    m_pModelInterFace->CheckModel();
    assert( m_pModelInterFace->IsEqual( * pModelSave2 ) );
#endif
}

bool CommandStack::UndoCommand( )
{
    wcout << L"CommandStack::UndoCommand " << L"index =" << m_iIndex << endl;
    try
    {
        set2OlderCmd();
        if ( m_bCombineCmds )
        {
            if ( isCloseBracketCmd() )
            {
                m_bBracketOpen = true;
                set2OlderCmd();
            }
            if ( m_bBracketOpen )
            {
                while ( ! isOpenBracketCmd() )
                    undoAndSetToOlder();
                m_bBracketOpen = false;
            }
            else
            {
                undoAndSetToOlder();
            }
        }
        else
        {
            while ( isBracketCmd( ) ) // skip bracket commands
                set2OlderCmd( );
            undoAndSetToOlder();
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
    wcout << L"CommandStack::RedoCommand " << L"index =" << m_iIndex << endl;
    try
    {
        if ( m_bCombineCmds ) 
        {
            if ( isOpenBracketCmd() )
            {
                m_bBracketOpen = true;
                set2YoungerCmd();
            }
            if ( m_bBracketOpen )
            {
                while ( ! isCloseBracketCmd() )
                    doAndSetToYounger();
                m_bBracketOpen = false;
            }
            else
                doAndSetToYounger();
        }
        else
        {
            while ( isBracketCmd( ) ) // skip bracket commands
                set2YoungerCmd();
            doAndSetToYounger();
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
