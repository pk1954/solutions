// DiscBaseKnotCmd.cpp
//
// NNetModel

#include "stdafx.h"
#include "DiscBaseKnotCmd.h"

using std::wcout;
using std::endl;
using std::move;

DiscBaseKnotCmd::DiscBaseKnotCmd
( 
    NNetModelWriterInterface & nmwi,
    NobId                const idBaseKnot, 
    bool                 const bDelete 
)
  : m_idBaseKnot(idBaseKnot),
    m_bDelete( bDelete )
{}

void DiscBaseKnotCmd::initialize(NNetModelWriterInterface & nmwi)
{
    m_pBaseKnot = nmwi.GetNobPtr<BaseKnot *>( m_idBaseKnot );
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    m_idEndKnots  .Resize( m_pBaseKnot->m_connections.GetNrOfIncomingConnections() );
    m_idStartKnots.Resize( m_pBaseKnot->m_connections.GetNrOfOutgoingConnections() );

    MicroMeterPnt umPos { m_pBaseKnot->GetPos() };
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
    if ( m_pBaseKnot->IsKnot() )
        m_bDelete = true;
}

void DiscBaseKnotCmd::Do( NNetModelWriterInterface & nmwi )
{
    if ( ! m_bInitialized )
    {
        initialize(nmwi);
        m_bInitialized = true;
    }

    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    for ( int i = 0; i < m_startKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot { m_startKnots[i] };
        if ( upKnot.get() )
        {
            Pipe & pipeOut { upKnot->m_connections.GetFirstOutgoing() };
            pipeOut.SetStartKnot( upKnot.get() );
            pipeOut.DislocateStartPoint();                     // dislocate new knot
            m_idStartKnots.SetAt(i, nmwi.Add2Model(move(upKnot)));
        }
    }
    for ( int i = 0; i < m_endKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot { m_endKnots[i] };
        if ( upKnot.get() )
        {
            Pipe & pipeIn { upKnot->m_connections.GetFirstIncoming() };
            pipeIn.SetEndKnot( upKnot.get() );
            pipeIn.DislocateEndPoint();                       // dislocate new knot 
            m_idEndKnots.SetAt(i, nmwi.Add2Model(move(upKnot)));
        }
    }
    m_pBaseKnot->ClearConnections();
    m_pBaseKnot->Check();
    if ( m_bDelete )
        m_upBaseKnot = nmwi.RemoveFromModel<BaseKnot>( * m_pBaseKnot );
}

void DiscBaseKnotCmd::Undo( NNetModelWriterInterface & nmwi )
{
    if ( ! m_pBaseKnot )   // might have been deleted earlier
        return;

    for ( int i = Cast2Int(m_idEndKnots.Size()) - 1; i >= 0; --i )
    {
        NobId idEndKnot { m_idEndKnots.Get(i) };
        Knot  & knotEnd   { * nmwi.GetNobPtr<Knot *>(idEndKnot) };
        Pipe  & pipeIn    { knotEnd.m_connections.GetFirstIncoming() };
        pipeIn.SetEndKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddIncoming( & pipeIn );
        m_endKnots[i] = nmwi.GetUPNobs().Pop<Knot>();
    }
    for ( int i = Cast2Int(m_startKnots.size()) - 1; i >= 0; --i )
    {
        NobId idStartKnot { m_idStartKnots.Get(i) };
        Knot  & knotStart   { * nmwi.GetNobPtr<Knot *>(idStartKnot) };
        Pipe  & pipeOut     { knotStart.m_connections.GetFirstOutgoing() };
        pipeOut.SetStartKnot( m_pBaseKnot );
        m_pBaseKnot->m_connections.AddOutgoing( & pipeOut );
        m_startKnots[i] = nmwi.GetUPNobs().Pop<Knot>();
    }
    if ( m_bDelete ) 
        m_upBaseKnot = nmwi.ReplaceInModel<BaseKnot,BaseKnot>(move(m_upBaseKnot));
}
