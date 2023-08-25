// Add2Connector.cpp
//
// NNetCommands

module;

#include <cassert>
#include <algorithm>
#include <memory>
#include <vector>

module NNetCommands:Add2Connector;

import Types;
import NNetModel;

using std::make_unique;
using std::ranges::sort;
using std::vector;

Add2Connector::Add2Connector
(
    NobId const id1,
    NobId const id2
)
  : m_id1(id1),
    m_id2(id2)
{
    vector<IoLine*> ioLines {};

    ioLines.clear();
    add2IoLines(id1, ioLines);
    add2IoLines(id2, ioLines);

    MicroMeterLine umLine { CalcMaxDistLine<IoLine>(ioLines) };
    if (umLine.IsZero())
        return;

    MicroMeterLine orthoLine { umLine.OrthoLine() };
    sort
    (
        ioLines,
        [&orthoLine](auto p1, auto p2)
        {
            return PointToLine(orthoLine, p1->GetPos()) < PointToLine(orthoLine, p2->GetPos());
        }
    );

    align(umLine, ioLines);

    if (ioLines.front()->IsInputLine())
        m_upIoConnectorResult = make_unique<InputConnector>(move(ioLines));
    else
        m_upIoConnectorResult = make_unique<OutputConnector>(move(ioLines));
}

void Add2Connector::Do()
{
    NNetCommand::Do();

    m_pNMWI->DeselectAllNobs();

    if (m_pNMWI->IsIoConnector(m_id1))
        m_upIoConnector1 = m_pNMWI->RemoveFromModel<IoConnector>(m_id1);
    if (m_pNMWI->IsIoConnector(m_id2))
        m_upIoConnector2 = m_pNMWI->RemoveFromModel<IoConnector>(m_id2);

    m_upIoConnectorResult->ConnectIoLines();
    m_pNMWI->Push2Model(move(m_upIoConnectorResult));
}

void Add2Connector::Undo()
{
    m_pNMWI->DumpModel(__FILE__, __LINE__);

    m_upIoConnectorResult = m_pNMWI->PopFromModel<IoConnector>();
    m_upIoConnectorResult->DisconnectIoLines();

    if (m_upIoConnector2)
    {
        m_upIoConnector2->ConnectIoLines();
        m_pNMWI->Restore2Model(move(m_upIoConnector2));
    }
    if (m_upIoConnector1)
    {
        m_upIoConnector1->ConnectIoLines();
        m_pNMWI->Restore2Model(move(m_upIoConnector1));
    }

    NNetCommand::Undo();
    m_pNMWI->DumpModel(__FILE__, __LINE__);
}

void Add2Connector::add2IoLines
(
    NobId const      idNob,
    vector<IoLine*> &ioLines
)
{
    Nob* pNob { m_pNMWI->GetNob(idNob) };
    if (pNob->IsIoConnector())
    {
        static_cast<IoConnector*>(pNob)->Apply2All([&ioLines](IoLine& io) { ioLines.push_back(&io); });
    }
    else if (pNob->IsIoLine())
    {
        ioLines.push_back(static_cast<InputLine*>(pNob));
    }
    else
        assert(false);
}

void Add2Connector::align
(
    MicroMeterLine  const& umLine, 
    vector<IoLine*> const& ioLines
)
{
    MicroMeter    const umDist             { NEURON_RADIUS * 2.0f };        
    MicroMeterPnt const umVector           { umLine.GetVector() };
    float         const fGapCount          { Cast2Float(ioLines.size() - 1) };
    MicroMeter    const umLineLengthTarget { umDist * fGapCount };
    MicroMeterPnt const umPntSingleVector  { umVector.ScaledTo(umDist) };
    MicroMeterPnt const umPntLineTarget    { umVector.ScaledTo(umLineLengthTarget) };
    MicroMeterPnt       umPnt              { umLine.GetCenter() - umPntLineTarget * 0.5f };
    for (auto& elem : ioLines)
    {
        elem->SetPos(umPnt);
        umPnt += umPntSingleVector;
    }
}
