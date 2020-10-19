// CommandStack.cpp
//
// Commands

#include "stdafx.h"
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
    NNetModel const * pModelSave1 { new NNetModel( m_pModelInterFace->GetModel( ) ) };
    m_pModelInterFace->CheckModel();
    pModelSave1->CheckModel();
#endif
    clearRedoStack( );
    pCmd->Do( * m_pModelInterFace );
    pushNewCommand( move(pCmd) );
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
    wcout << L"# CommandStack::UndoCommand " << L"index =" << m_iIndex << endl;
    if ( UndoStackEmpty() )
       return false;
    if ( m_bCombineCmds )
    {
        set2OlderCmd();
        if ( isCloseBracketCmd() )
        {
            set2OlderCmd();
            m_bIndexInSeries = true;
        }
        if ( m_bIndexInSeries )
        { 
            do
            {
                undoCmd( );
                set2OlderCmd();
            } while ( ! isOpenBracketCmd() );
            m_bIndexInSeries = false;            // index is on open bracket
        }
        else
        {
            undoCmd( );
        }
    }
    else
    {
        do
        {
            set2OlderCmd( );
            if ( UndoStackEmpty() )
                return false;
        } while ( isBracketCmd() ); // skip bracket commands
        undoCmd( );
    }
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}

bool CommandStack::RedoCommand( )
{
    wcout << L"# CommandStack::RedoCommand " << L"index =" << m_iIndex << endl;
    if ( RedoStackEmpty() ) 
        return false;
    if ( m_bCombineCmds ) 
    {
        if ( isOpenBracketCmd() )
        {
            set2YoungerCmd();
            m_bIndexInSeries = true;
        }
        if ( m_bIndexInSeries )
        { 
            for (;;)
            {
                doAndSet2YoungerCmd();
                if ( RedoStackEmpty() )
                    break;
                if ( isCloseBracketCmd() )
                {
                    set2YoungerCmd();
                    m_bIndexInSeries = false; 
                    break;
                }
            }
        }
        else
        {
            doAndSet2YoungerCmd();
        }
    }
    else
    {
        while ( isBracketCmd() ) // skip bracket commands
        {
            set2YoungerCmd();
            if ( RedoStackEmpty() ) 
                return false;
        } 
        doAndSet2YoungerCmd();
    }
    m_pModelInterFace->StaticModelChanged( );
    m_pObservable->NotifyAll( true );
    return true;
}
