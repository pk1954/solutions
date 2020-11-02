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
            auto upKnotNew { make_unique<Knot>( umPos ) };
            upKnotNew->m_connections.AddIncoming( & pipe );  // prepare new knot as far as possible
            m_endKnots.push_back( move(upKnotNew) );         // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_endKnots have their incoming pipe set
    m_pBaseKnot->m_connections.Apply2AllOutPipes
    ( 
        [&]( Pipe & pipe ) // every outgoing pipe needs a new start knot
        { 
            auto upKnotNew { make_unique<Knot>( umPos ) };
            upKnotNew->m_connections.AddOutgoing( & pipe );  // prepare new knot as far as possible
            m_startKnots.push_back( move(upKnotNew) );       // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_startKnots have their outgoing pipe set
    m_idEndKnots  .resize( m_endKnots  .size(), NO_SHAPE );
    m_idStartKnots.resize( m_startKnots.size(), NO_SHAPE );
    if ( m_pBaseKnot->IsKnot() )
        m_bDelete = true;
}

void DisconnectBaseKnotCommand::Do( NNetModelWriterInterface & nmwi )
{
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    wcout << L"DisconnectBaseKnotCommand " << L"Do " << L"shapeId = " << m_pBaseKnot->GetId( ) << endl;
//    for ( auto & upKnot : m_startKnots )
    for ( int i = 0; i < m_startKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot  { m_startKnots[i] };
        Pipe             & pipeOut { upKnot->m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( upKnot.get() );
        pipeOut.DislocateStartPoint( );                     // dislocate new knot
        m_idStartKnots[i] = nmwi.Add2Model( move(upKnot) );
    }
    for ( int i = 0; i < m_endKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot { m_endKnots[i] };
        Pipe             & pipeIn { upKnot->m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( upKnot.get() );
        pipeIn.DislocateEndPoint( );                       // dislocate new knot 
        m_idEndKnots[i] = nmwi.Add2Model( move(upKnot) );
    }
    m_pBaseKnot->ClearConnections();
    if ( m_bDelete )
        m_upBaseKnot = nmwi.RemoveFromModel<BaseKnot>( m_pBaseKnot->GetId() );
}

void DisconnectBaseKnotCommand::Undo( NNetModelWriterInterface & nmwi )
{
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    wcout << L"DisconnectBaseKnotCommand " << L"Undo " << L"shapeId = " << m_pBaseKnot->GetId( ) << endl;
    for ( int i = 0; i < m_startKnots.size(); ++i )
    {
        ShapeId idStartKnot { m_idStartKnots[i] };
        Knot  & knotStart   { * nmwi.GetShapePtr<Knot *>(idStartKnot) };
        Pipe  & pipeOut     { knotStart.m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddOutgoing( & pipeOut );
        m_startKnots[i] = nmwi.RemoveFromModel<Knot>( idStartKnot );
    }
    for ( int i = 0; i < m_endKnots.size(); ++i )
    {
        ShapeId idEndKnot { m_idEndKnots[i] };
        Knot  & knotEnd   { * nmwi.GetShapePtr<Knot *>(idEndKnot) };
        Pipe  & pipeIn    { knotEnd.m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddIncoming( & pipeIn );
        m_endKnots[i] = nmwi.RemoveFromModel<Knot>( idEndKnot );
    }
    if ( m_bDelete ) 
        m_upBaseKnot = nmwi.Store2Model<BaseKnot>( move( m_upBaseKnot ) );
}
