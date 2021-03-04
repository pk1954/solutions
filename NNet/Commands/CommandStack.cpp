// CommandStack.cpp
//
// Commands

#include "stdafx.h"
#include "assert.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"

using std::wcout;
using std::endl;

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
    Observable               * const pStaticModelObservable 
)
{
    m_pNMWI = pModel;
    m_pStaticModelObservable = pStaticModelObservable;
}

void CommandStack::Clear( )
{
    m_iIndex = 0;
    m_bIndexInSeries = false;
    clearRedoStack();
    m_pStaticModelObservable->NotifyAll( true );
}

void CommandStack::clearRedoStack( )
{
    m_CommandStack.erase( m_CommandStack.begin() + m_iIndex, m_CommandStack.end() );
    assert( RedoStackEmpty() );
}

void CommandStack::pushNewCommand( unique_ptr<Command> pCmd )
{
    if ( m_bIndexInSeries )
    {
        assert( m_iIndex > 0 );
        if ( ! canBeCombined( * pCmd, * m_CommandStack[m_iIndex-1] ) )
            CloseSeries( ); 
    }

    if ( pCmd->IsMoveCommand( ) && ! m_bIndexInSeries )
        OpenSeries( ); 

    push( move(pCmd) );
}

void CommandStack::PushCommand( unique_ptr<Command> pCmd )
{
#ifdef _DEBUG
    NNetModel const & model { m_pNMWI->GetModel( ) };
    m_pNMWI->CheckModel();
    NNetModel modelSave1( model );
    m_pNMWI->CheckModel();
#endif
    clearRedoStack( );
    assert( * pCmd );
    pCmd->Do( * m_pNMWI );
    m_pNMWI->CheckModel();
    pushNewCommand( move(pCmd) );
    m_pStaticModelObservable->NotifyAll( true );

#ifdef _DEBUG
    NNetModel modelSave2( model );
    modelSave2.CheckModel();
    m_pNMWI->CheckModel();
    UndoCommand();
    m_pNMWI->CheckModel();
    if ( !(model == modelSave1) )
    {
        int x = 42;
    }
    m_pNMWI->CheckModel();
    RedoCommand();
    m_pNMWI->CheckModel();
    if ( !(model == modelSave2) )
    {
        int x = 42;
    }
#endif
}

bool CommandStack::UndoCommand( )
{
//    wcout << Scanner::COMMENT_SYMBOL << L" CommandStack::UndoCommand " << L"index =" << m_iIndex << endl;
    if ( UndoStackEmpty() )
       return false;
    set2OlderCmd();
    if ( isCloseBracketCmd() )
    {
        set2OlderCmd();                     // skip closing bracket
        m_bIndexInSeries = true;            // we are in a series
    }
    if ( m_bIndexInSeries )                 // we are in a series, either
    {                                       // because we just found closing bracket 
        do                                  // or we are at top of stack
        {
            undoCmd( );                     // undo all commands in series
            m_pNMWI->CheckModel();
            set2OlderCmd();
        } while ( ! isOpenBracketCmd() );   // until opening bracket is reached
        m_bIndexInSeries = false;           // index is on open bracket
    }
    else                                    // we are not in a series
    {                                       // normal processing
        undoCmd( );                         // of one command
        m_pNMWI->CheckModel();
    }
    m_pStaticModelObservable->NotifyAll( true );
    return true;
}

bool CommandStack::RedoCommand( )
{
//    wcout << Scanner::COMMENT_SYMBOL << L" CommandStack::RedoCommand " << L"index =" << m_iIndex << endl;
    if ( RedoStackEmpty() ) 
        return false;
    if ( isOpenBracketCmd() )
    {
        set2YoungerCmd();                   // skip opening bracket
        m_bIndexInSeries = true;            // we are in a series
        for (;;)                            // process series in loop
        {                                  
            doAndSet2YoungerCmd();         
            if ( RedoStackEmpty() )         // we reached top of stack 
                break;                      // redo finished
            if ( isCloseBracketCmd() )      // we found closing bracket
            {
                set2YoungerCmd();           // skip closing bracket
                m_bIndexInSeries = false;   // we left the series
                break;                      // redo finished
            }
        }
    }
    else                                    // no opening bracket
    {                                       // normal processing
        doAndSet2YoungerCmd();              // of one command
    }
    m_pStaticModelObservable->NotifyAll( true );
    return true;
}
