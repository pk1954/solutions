// DiscBaseKnotCmd.cpp
//
// NNetModel

#include "stdafx.h"
#include "DiscBaseKnotCmd.h"

using std::wcout;
using std::endl;
using std::make_unique;

DiscBaseKnotCmd::DiscBaseKnotCmd
(
    Nob      & nob,
    bool const bDelete 
)
  : m_baseKnot(*Cast2BaseKnot(&nob)),
    m_bDelete(bDelete)
{
    m_idEndKnots  .Resize(m_baseKnot.GetNrOfIncomingConnections());
    m_idStartKnots.Resize(m_baseKnot.GetNrOfOutgoingConnections());

    m_umPos = m_baseKnot.GetPos();
    m_baseKnot.Apply2AllInPipes
    (
        [this](Pipe & pipe) // every incoming pipe needs a new end knot
        { 
            auto upKnotNew { make_unique<Knot>(m_umPos) };
            upKnotNew->Select(pipe.IsSelected());
            upKnotNew->AddIncoming(& pipe);          // prepare new knot as far as possible
            m_endKnots.push_back(move(upKnotNew));   // store new knot for later
        }                                            // but do not touch m_pBaseKnot
    );  // Knots in m_endKnots have their incoming pipe set
    m_baseKnot.Apply2AllOutPipes
    (
        [this](Pipe & pipe) // every outgoing pipe needs a new start knot
        { 
            auto upKnotNew { make_unique<Knot>(m_umPos) };
            upKnotNew->Select(pipe.IsSelected());
            upKnotNew->AddOutgoing(& pipe);            // prepare new knot as far as possible
            m_startKnots.push_back(move(upKnotNew));   // store new knot for later
        }                                              // but do not touch m_pBaseKnot
    );  // Knots in m_startKnots have their outgoing pipe set
    if (m_baseKnot.IsKnot() || m_baseKnot.IsNeuron())
        m_bDelete = true;
}

void DiscBaseKnotCmd::Do()
{
    for (size_t i = 0; i < m_startKnots.size(); ++i)
    {
        unique_ptr<Knot> & upKnot { m_startKnots[i] };
        if (upKnot.get())
        {
            Pipe & pipeOut { upKnot->GetFirstOutgoing() };
            pipeOut.SetStartKnot(upKnot.get());
            pipeOut.DislocateStartPoint();                     // dislocate new knot
            m_idStartKnots.SetAt(i, m_pNMWI->Push2Model(move(upKnot)));
        }
    }
    for (size_t i = 0; i < m_endKnots.size(); ++i)
    {
        unique_ptr<Knot> & upKnot { m_endKnots[i] };
        if (upKnot.get())
        {
            Pipe & pipeIn { upKnot->GetFirstIncoming() };
            pipeIn.SetEndKnot(upKnot.get());
            pipeIn.DislocateEndPoint();                       // dislocate new knot 
            m_idEndKnots.SetAt(i, m_pNMWI->Push2Model(move(upKnot)));
        }
    }
    m_baseKnot.ClearConnections();
    if (m_bDelete)
    {
        m_upBaseKnot = m_pNMWI->RemoveFromModel<BaseKnot>(m_baseKnot);
        assert(m_upBaseKnot);
    }
}

void DiscBaseKnotCmd::Undo()
{
    for (size_t i = m_endKnots.size(); i --> 0;)
    {
        NobId   idEndKnot { m_idEndKnots.Get(i) };
        Knot  & knotEnd   { * m_pNMWI->GetNobPtr<Knot *>(idEndKnot) };
        Pipe  & pipeIn    { knotEnd.GetFirstIncoming() };
        pipeIn.SetEndKnot(&m_baseKnot);
        m_baseKnot.AddIncoming(& pipeIn);
        m_endKnots[i] = m_pNMWI->PopFromModel<Knot>();
    }
    for (size_t i = m_startKnots.size(); i --> 0;)
    {
        NobId   idStartKnot { m_idStartKnots.Get(i) };
        Knot  & knotStart   { * m_pNMWI->GetNobPtr<Knot *>(idStartKnot) };
        Pipe  & pipeOut     { knotStart.GetFirstOutgoing() };
        pipeOut.SetStartKnot(&m_baseKnot);
        m_baseKnot.AddOutgoing(& pipeOut);
        m_startKnots[i] = m_pNMWI->PopFromModel<Knot>();
    }
    if (m_bDelete) 
    {
        assert(m_upBaseKnot);
        m_upBaseKnot = m_pNMWI->ReplaceInModel<BaseKnot>(move(m_upBaseKnot));
    }
}
