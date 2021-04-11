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
    ShapeId const idBaseKnot, 
    bool    const bDelete 
)
  : m_idBaseKnot(idBaseKnot),
    m_bDelete( bDelete )
{}

void DisconnectBaseKnotCommand::init( NNetModelWriterInterface & nmwi )
{ 
    m_pBaseKnot = nmwi.GetShapePtr<BaseKnot *>( m_idBaseKnot );
    
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
    m_pBaseKnot->m_connections.Apply2AllInPipes
    ( 
        [&]( Pipe & pipe ) // every incoming pipe needs a new end knot
        { 
            auto upKnotNew { make_unique<Knot>( umPos ) };
            upKnotNew->Select( pipe.IsSelected(), false );
            upKnotNew->m_connections.AddIncoming( & pipe );  // prepare new knot as far as possible
            m_endKnots.push_back( move(upKnotNew) );         // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_endKnots have their incoming pipe set
    m_pBaseKnot->m_connections.Apply2AllOutPipes
    ( 
        [&]( Pipe & pipe ) // every outgoing pipe needs a new start knot
        { 
            auto upKnotNew { make_unique<Knot>( umPos ) };
            upKnotNew->Select( pipe.IsSelected(), false );
            upKnotNew->m_connections.AddOutgoing( & pipe );  // prepare new knot as far as possible
            m_startKnots.push_back( move(upKnotNew) );       // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_startKnots have their outgoing pipe set
    m_idEndKnots  .Resize( m_endKnots  .size() );
    m_idStartKnots.Resize( m_startKnots.size() );
    if ( m_pBaseKnot->IsKnot() )
        m_bDelete = true;
    m_bInitialized = true;
}

void DisconnectBaseKnotCommand::Do( NNetModelWriterInterface & nmwi )
{
    if ( ! m_bInitialized )
        init( nmwi );

    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    for ( int i = 0; i < m_startKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot  { m_startKnots[i] };
        Pipe             & pipeOut { upKnot->m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( upKnot.get() );
        pipeOut.DislocateStartPoint();                     // dislocate new knot
        m_idStartKnots.SetAt(i, nmwi.GetUPShapes().Push(move(upKnot)));
    }
    for ( int i = 0; i < m_endKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot { m_endKnots[i] };
        Pipe             & pipeIn { upKnot->m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( upKnot.get() );
        pipeIn.DislocateEndPoint();                       // dislocate new knot 
        m_idEndKnots.SetAt(i, nmwi.GetUPShapes().Push(move(upKnot)));
    }
    m_pBaseKnot->ClearConnections();
    if ( m_bDelete )
        m_upBaseKnot = nmwi.RemoveFromModel<BaseKnot>( * m_pBaseKnot );
}

void DisconnectBaseKnotCommand::Undo( NNetModelWriterInterface & nmwi )
{
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    for ( int i = Cast2Int(m_idEndKnots.Size()) - 1; i >= 0; --i )
    {
        ShapeId idEndKnot { m_idEndKnots.Get(i) };
        Knot  & knotEnd   { * nmwi.GetShapePtr<Knot *>(idEndKnot) };
        Pipe  & pipeIn    { knotEnd.m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddIncoming( & pipeIn );
        m_endKnots[i] = nmwi.GetUPShapes().Pop<Knot>();
    }
    for ( int i = Cast2Int(m_startKnots.size()) - 1; i >= 0; --i )
    {
        ShapeId idStartKnot { m_idStartKnots.Get(i) };
        Knot  & knotStart   { * nmwi.GetShapePtr<Knot *>(idStartKnot) };
        Pipe  & pipeOut     { knotStart.m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddOutgoing( & pipeOut );
        m_startKnots[i] = nmwi.GetUPShapes().Pop<Knot>();
    }
    if ( m_bDelete ) 
        m_upBaseKnot = nmwi.ReplaceInModel<BaseKnot,BaseKnot>(move(m_upBaseKnot));
}
