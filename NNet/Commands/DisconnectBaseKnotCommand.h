// DisconnectBaseKnotCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "ShapeId.h"
#include "BaseKnot.h"
#include "Knot.h"

using std::vector;
using std::wcout;
using std::endl;

// DisconnectBaseKnot and DeleteBaseKnot are nearly identical.
// For Delete... functioniality create DisconnectBaseKnot 
// with 3rd parameter set to true.

class DisconnectBaseKnotCommand : public Command
{
public:
    DisconnectBaseKnotCommand
    ( 
        NNetModelWriterInterface & model, 
        ShapeId              const id, 
        bool                 const bDelete 
    )
      :	m_pBaseKnot( model.GetShapePtr<BaseKnot *>( id ) ),
        m_bDelete( bDelete )
    { 
        wcout << L"DisconnectBaseKnotCommand " << L" shapeId = " << m_pBaseKnot->GetId( ) << endl;
        MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
        m_pBaseKnot->m_connections.Apply2AllInPipes
        ( 
            [&]( Pipe & pipe ) // every incoming pipe needs a new end knot
            { 
                Knot * pKnotNew { model.NewBaseKnot<Knot>( umPos ) };
                pKnotNew->m_connections.AddIncoming( & pipe );  // prepare new knot as far as possible
                pipe.DislocateEndPoint( );                      // dislocate new knot 
                m_endKnots.push_back( pKnotNew );               // store new knot for later
            }                                                   // but do not touch m_pBaseKnot
        );  // Knots in m_endKnots have their incoming pipe set
        m_pBaseKnot->m_connections.Apply2AllOutPipes
        ( 
            [&]( Pipe & pipe ) // every outgoing pipe needs a new start knot
            { 
                Knot * pKnotNew { model.NewBaseKnot<Knot>( umPos ) };
                pKnotNew->m_connections.AddOutgoing( & pipe );  // prepare new knot as far as possible
                pipe.DislocateStartPoint( );                    // dislocate new knot 
                m_startKnots.push_back( pKnotNew );             // store new knot for later
            }                                                   // but do not touch m_pBaseKnot
        );  // Knots in m_startKnots have their outgoing pipe set
    }

    ~DisconnectBaseKnotCommand( )
    {
        for ( Knot * pKnot : m_startKnots )
            delete pKnot;
        for ( Knot * pKnot : m_endKnots )
            delete pKnot;
    }

    virtual void Do( NNetModelWriterInterface & model )
    {
        wcout << L"DisconnectBaseKnotCommand " << L"Do " << L"shapeId = " << m_pBaseKnot->GetId( ) << endl;
        for ( Knot * pKnot : m_startKnots )
        {
            Pipe & pipeOut { pKnot->m_connections.GetFirstOutgoing() };
            pipeOut.SetStartKnot( pKnot );
            model.Store2Model( pKnot );
        }
        for ( Knot * pKnot : m_endKnots )
        {
            Pipe & pipeIn { pKnot->m_connections.GetFirstIncoming() };
            pipeIn.SetEndKnot( pKnot );
            model.Store2Model( pKnot );
        }
        m_pBaseKnot->ClearConnections();
        if ( m_bDelete || m_pBaseKnot->IsKnot() )
            model.RemoveFromModel( m_pBaseKnot );
    }

    virtual void Undo( NNetModelWriterInterface & model )
    {
        wcout << L"DisconnectBaseKnotCommand " << L"Undo " << L"shapeId = " << m_pBaseKnot->GetId( ) << endl;
        for ( Knot * pKnot : m_startKnots )
        {
            Pipe & pipeOut { pKnot->m_connections.GetFirstOutgoing() };
            pipeOut.SetStartKnot( m_pBaseKnot );
            m_pBaseKnot->m_connections.AddOutgoing( & pipeOut );
            model.RemoveFromModel( pKnot );
        }
        for ( Knot * pKnot : m_endKnots )
        {
            Pipe & pipeIn { pKnot->m_connections.GetFirstIncoming() };
            pipeIn.SetEndKnot( m_pBaseKnot );
            m_pBaseKnot->m_connections.AddIncoming( & pipeIn );
            model.RemoveFromModel( pKnot );
        }
        if ( m_bDelete || m_pBaseKnot->IsKnot() )
            model.Store2Model( m_pBaseKnot );
    }

private:
    BaseKnot     * m_pBaseKnot;
    vector<Knot *> m_startKnots { };
    vector<Knot *> m_endKnots   { };
    bool           m_bDelete; // true: delete BaseKnot, false: disconnect only
};
