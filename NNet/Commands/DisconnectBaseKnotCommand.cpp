// DisconnectBaseKnotCommand.cpp
//
// NNetModel

#include "stdafx.h"
#include "DisconnectBaseKnotCommand.h"

using std::wcout;
using std::endl;
using std::move;

DisconnectBaseKnotCommand::DisconnectBaseKnotCommand
( 
    NNetModelWriterInterface & nmwi, 
    ShapeId              const id, 
    bool                 const bDelete 
)
    :	m_pBaseKnot( nmwi.GetShapePtr<BaseKnot *>( id ) ),
    m_bDelete( bDelete )
{ 
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    wcout << L"DisconnectBaseKnotCommand " << L" shapeId = " << m_pBaseKnot->GetId( ) << endl;
    MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
    m_pBaseKnot->m_connections.Apply2AllInPipes
    ( 
        [&]( Pipe & pipe ) // every incoming pipe needs a new end knot
        { 
            unique_ptr<Knot> upKnotNew { nmwi.NewBaseKnot<Knot>( umPos ) };
            upKnotNew->m_connections.AddIncoming( & pipe );  // prepare new knot as far as possible
            pipe.DislocateEndPoint( );                       // dislocate new knot 
           m_endKnots.push_back( move(upKnotNew) );          // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_endKnots have their incoming pipe set
    m_pBaseKnot->m_connections.Apply2AllOutPipes
    ( 
        [&]( Pipe & pipe ) // every outgoing pipe needs a new start knot
        { 
            unique_ptr<Knot> upKnotNew { nmwi.NewBaseKnot<Knot>( umPos ) };
            upKnotNew->m_connections.AddOutgoing( & pipe );  // prepare new knot as far as possible
            pipe.DislocateStartPoint( );                     // dislocate new knot
            m_startKnots.push_back( move(upKnotNew) );       // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_startKnots have their outgoing pipe set
}

void DisconnectBaseKnotCommand::Do( NNetModelWriterInterface & nmwi )
{
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    wcout << L"DisconnectBaseKnotCommand " << L"Do " << L"shapeId = " << m_pBaseKnot->GetId( ) << endl;
    for ( auto & upKnot : m_startKnots )
    {
        Pipe & pipeOut { upKnot->m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( upKnot.get() );
        nmwi.Store2Model( move(upKnot) );
    }
    for ( auto & upKnot : m_endKnots )
    {
        Pipe & pipeIn { upKnot->m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( upKnot.get() );
        nmwi.Store2Model( move(upKnot) );
    }
    m_pBaseKnot->ClearConnections();
    if ( m_bDelete || m_pBaseKnot->IsKnot() )
        m_upBaseKnot = move(nmwi.RemoveFromModel<BaseKnot>( m_pBaseKnot->GetId() ));
}

void DisconnectBaseKnotCommand::Undo( NNetModelWriterInterface & nmwi )
{
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    wcout << L"DisconnectBaseKnotCommand " << L"Undo " << L"shapeId = " << m_pBaseKnot->GetId( ) << endl;
    for ( auto & upKnot : m_startKnots )
    {
        Pipe & pipeOut { upKnot->m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddOutgoing( & pipeOut );
        upKnot = move(nmwi.RemoveFromModel<Knot>( upKnot->GetId() ));
    }
    for ( auto & upKnot : m_endKnots )
    {
        Pipe & pipeIn { upKnot->m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddIncoming( & pipeIn );
        upKnot = nmwi.RemoveFromModel<Knot>( upKnot->GetId() );
    }
    if ( m_bDelete || m_pBaseKnot->IsKnot() ) 
        m_upBaseKnot = move(nmwi.Store2Model<BaseKnot>( move( m_upBaseKnot ) ));
}
