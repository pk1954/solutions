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
    Nob      & nob,
    bool const bDelete 
)
  : m_bDelete(bDelete),
    m_baseKnot(*Cast2BaseKnot(&nob))
{
    m_idEndKnots  .Resize( m_baseKnot.GetNrOfIncomingConnections() );
    m_idStartKnots.Resize( m_baseKnot.GetNrOfOutgoingConnections() );

    MicroMeterPnt umPos { m_baseKnot.GetPos() };
    m_baseKnot.Apply2AllInPipes
    ( 
        [&]( Pipe & pipe ) // every incoming pipe needs a new end knot
        { 
            auto upKnotNew { make_unique<Knot>( umPos ) };
            upKnotNew->Select(pipe.IsSelected());
            upKnotNew->AddIncoming(& pipe);  // prepare new knot as far as possible
            m_endKnots.push_back( move(upKnotNew) );         // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_endKnots have their incoming pipe set
    m_baseKnot.Apply2AllOutPipes
    ( 
        [&]( Pipe & pipe ) // every outgoing pipe needs a new start knot
        { 
            auto upKnotNew { make_unique<Knot>( umPos ) };
            upKnotNew->Select(pipe.IsSelected());
            upKnotNew->AddOutgoing(& pipe);  // prepare new knot as far as possible
            m_startKnots.push_back( move(upKnotNew) );       // store new knot for later
        }                                                    // but do not touch m_pBaseKnot
    );  // Knots in m_startKnots have their outgoing pipe set
    if ( m_baseKnot.IsKnot() )
        m_bDelete = true;
}

void DiscBaseKnotCmd::Do( NNetModelWriterInterface & nmwi )
{
    for ( int i = 0; i < m_startKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot { m_startKnots[i] };
        if ( upKnot.get() )
        {
            Pipe & pipeOut { upKnot->GetFirstOutgoing() };
            pipeOut.SetStartKnot( upKnot.get() );
            pipeOut.DislocateStartPoint();                     // dislocate new knot
            m_idStartKnots.SetAt(i, nmwi.Push2Model(move(upKnot)));
        }
    }
    for ( int i = 0; i < m_endKnots.size(); ++i )
    {
        unique_ptr<Knot> & upKnot { m_endKnots[i] };
        if ( upKnot.get() )
        {
            Pipe & pipeIn { upKnot->GetFirstIncoming() };
            pipeIn.SetEndKnot( upKnot.get() );
            pipeIn.DislocateEndPoint();                       // dislocate new knot 
            m_idEndKnots.SetAt(i, nmwi.Push2Model(move(upKnot)));
        }
    }
    m_baseKnot.ClearConnections();
    m_baseKnot.Check();
    if ( m_bDelete )
        m_upBaseKnot = nmwi.RemoveFromModel<BaseKnot>(m_baseKnot);
}

void DiscBaseKnotCmd::Undo( NNetModelWriterInterface & nmwi )
{
    for ( int i = Cast2Int(m_idEndKnots.Size()) - 1; i >= 0; --i )
    {
        NobId   idEndKnot { m_idEndKnots.Get(i) };
        Knot  & knotEnd   { * nmwi.GetNobPtr<Knot *>(idEndKnot) };
        Pipe  & pipeIn    { knotEnd.GetFirstIncoming() };
        pipeIn.SetEndKnot( &m_baseKnot );
        m_baseKnot.AddIncoming( & pipeIn );
        m_endKnots[i] = nmwi.PopFromModel<Knot>();
    }
    for ( int i = Cast2Int(m_startKnots.size()) - 1; i >= 0; --i )
    {
        NobId   idStartKnot { m_idStartKnots.Get(i) };
        Knot  & knotStart   { * nmwi.GetNobPtr<Knot *>(idStartKnot) };
        Pipe  & pipeOut     { knotStart.GetFirstOutgoing() };
        pipeOut.SetStartKnot(&m_baseKnot);
        m_baseKnot.AddOutgoing( & pipeOut );
        m_startKnots[i] = nmwi.PopFromModel<Knot>();
    }
    if ( m_bDelete ) 
        m_upBaseKnot = nmwi.ReplaceInModel<BaseKnot,BaseKnot>(move(m_upBaseKnot));
}
