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
    Observable               * const pObservable 
)
{
    m_pModelInterFace = pModel;
    m_pObservable = pObservable;
}

void CommandStack::Clear( )
{
    m_iIndex = 0;
    clearRedoStack();
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
    NNetModel const & model { m_pModelInterFace->GetModel( ) };
    unique_ptr<NNetModel const> pModelSave1 { make_unique<NNetModel>( model ) };
    m_pModelInterFace->CheckModel();
    //pModelSave1->CheckModel();
#endif
    clearRedoStack( );
    pCmd->Do( * m_pModelInterFace );
    pushNewCommand( move(pCmd) );
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );

#ifdef _DEBUG
    unique_ptr<NNetModel const> pModelSave2 { make_unique<NNetModel>( model ) };
    pModelSave2->CheckModel();
    m_pModelInterFace->CheckModel();
    UndoCommand();
    m_pModelInterFace->CheckModel();
    if ( !(model == * pModelSave1) )
    {
        int x = 42;
    }
    m_pModelInterFace->CheckModel();
    RedoCommand();
    m_pModelInterFace->CheckModel();
    if ( !(model == * pModelSave2) )
    {
        int x = 42;
    }
#endif
}

bool CommandStack::UndoCommand( )
{
//    wcout << L"# CommandStack::UndoCommand " << L"index =" << m_iIndex << endl;
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
            set2OlderCmd();
        } while ( ! isOpenBracketCmd() );   // until opening bracket is reached
        m_bIndexInSeries = false;           // index is on open bracket
    }
    else                                    // we are not in a series
    {                                       // normal processing
        undoCmd( );                         // of one command
    }
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}

bool CommandStack::RedoCommand( )
{
//    wcout << L"# CommandStack::RedoCommand " << L"index =" << m_iIndex << endl;
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

    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}
