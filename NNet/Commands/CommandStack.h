// CommandStack.h
//
// NNetModel

#pragma once

#include <vector>
#include <utility>
#include <typeinfo>
#include <exception>
#include "BoolOp.h"
#include "Observable.h"
#include "Command.h"
#include "MoveCommand.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

class NNetModelWriterInterface;

class CommandStack
{
public:

    void Initialize( NNetModelWriterInterface * const, Observable * const );

    bool UndoStackEmpty() const { return m_iIndex == 0; }
    bool RedoStackEmpty() const { return m_iIndex == m_CommandStack.size(); }

    void SetCombineCmdsFlag( bool const op ) { m_bCombineCmds = op; }
    bool GetCombineCmdsFlag( ) const { return m_bCombineCmds;  }

    void OpenSeries( )
    {
        push( make_unique<OpenBracket>( OpenBracket( ) ) );
        m_bIndexInSeries = true;
    }

    void CloseSeries( )
    {
        push( make_unique<CloseBracket>( CloseBracket( ) ) ); 
        m_bIndexInSeries = false;
    }

    void PushCommand( unique_ptr<Command> );
    bool UndoCommand( );
    bool RedoCommand( );

    void Clear( );

private:
    class OpenBracket  : public Command { };
    class CloseBracket : public Command { };

    vector<unique_ptr<Command>> m_CommandStack    { };
    size_t                      m_iIndex          { 0 };     // index into m_Commandstack
    bool                        m_bIndexInSeries  { false }; // index in series?
    bool                        m_bCombineCmds    { true };
    NNetModelWriterInterface  * m_pModelInterFace { nullptr };
    Observable                * m_pObservable     { nullptr };

    Command & getCurrentCmd( ) 
    { 
        return * m_CommandStack.at( m_iIndex ); 
    }

    void undoCmd() { getCurrentCmd().Undo( * m_pModelInterFace ); }
    void doCmd  () { getCurrentCmd().Do  ( * m_pModelInterFace ); }

    void push( unique_ptr<Command> pCmd )
    {
        m_CommandStack.push_back( move(pCmd) );
        ++m_iIndex;
    }

    void set2OlderCmd( )
    {
        assert( ! UndoStackEmpty() );
        --m_iIndex;
    }

    void set2YoungerCmd( )
    {
        assert( m_iIndex < m_CommandStack.size() );
        ++m_iIndex;
    }
    void pushNewCommand( unique_ptr<Command> );

    void clearRedoStack   ( );
    bool isOpenBracketCmd ( ) { return typeid( getCurrentCmd() ) == typeid(OpenBracket ); }
    bool isCloseBracketCmd( ) { return typeid( getCurrentCmd() ) == typeid(CloseBracket); }
    bool isBracketCmd     ( ) { return isOpenBracketCmd() || isCloseBracketCmd(); }
};
