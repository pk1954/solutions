// Add2ConnectorCmd.cpp
//
// NNetCommands

module;

#include <cassert>
#include <algorithm>
#include <memory>
#include <vector>

module NNetCommands:Add2ConnectorCmd;

import Types;
import NNetModel;

using std::make_unique;
using std::ranges::sort;
using std::vector;

Add2ConnectorCmd::Add2ConnectorCmd
(
    NobId const id1,
    NobId const id2
)
  : m_id1(id1),
    m_id2(id2)
{
    vector<IoLine*> ioLines;

    add2IoLines(ioLines, id1);
    add2IoLines(ioLines, id2);

    m_umLine = CalcMaxDistLine<IoLine>(ioLines);
    if (m_umLine.IsZero())
        return;

    sortIoLines(ioLines, m_umLine);

    if (ioLines.front()->IsInputLine())
        m_upIoConnectorResult = make_unique<InputConnector>(move(ioLines));
    else
        m_upIoConnectorResult = make_unique<OutputConnector>(move(ioLines));

    m_originalPositions = m_upIoConnectorResult->GetPositions();
}

void Add2ConnectorCmd::Do()
{
    m_pNMWI->DeselectAllNobs();

    remove(m_upIoConnector1, m_id1);
    remove(m_upIoConnector2, m_id2);

    m_upIoConnectorResult->AlignPositions(m_umLine);
    m_upIoConnectorResult->ConnectIoLines();
    m_pNMWI->Push2Model(move(m_upIoConnectorResult));
}

void Add2ConnectorCmd::Undo()
{
    m_upIoConnectorResult = m_pNMWI->PopFromModel<IoConnector>();
    m_upIoConnectorResult->SetPositions(m_originalPositions);  // restore original positions

    restore(m_upIoConnector2, m_id2);
    restore(m_upIoConnector1, m_id1);

    SelectionCommand::Undo();
}

void Add2ConnectorCmd::add2IoLines
(
    vector<IoLine*> &ioLines,
    NobId const      idNob
)
{
    Nob* pNob { m_pNMWI->GetNob(idNob) };
    if (pNob->IsIoConnector())
    {
        static_cast<IoConnector*>(pNob)->Apply2All
        (
            [this, &ioLines](IoLine& io) 
            { 
                ioLines.push_back(&io);
            }
        );
    }
    else if (pNob->IsIoLine())
    {
        ioLines.push_back(static_cast<InputLine*>(pNob));
    }
    else
        assert(false);
}

void Add2ConnectorCmd::remove
(
    unique_ptr<IoConnector>& upIoConnector,
    NobId             const  idNob
)
{
    if (m_pNMWI->IsIoConnector(idNob))
        upIoConnector = m_pNMWI->RemoveFromModel<IoConnector>(idNob);
}

void Add2ConnectorCmd::restore
(
    unique_ptr<IoConnector>& upIoConnector,
    NobId             const idNob
)
{
    if (upIoConnector)
    {
        upIoConnector->ConnectIoLines();
        m_pNMWI->Restore2Model(move(upIoConnector));
    }
    else if (IoLine * pIoLine { m_pNMWI->GetNobPtr<IoLine*>(idNob) })
    {
        pIoLine->DisconnectFromIoConnector();
    }
    else
        assert(false);
}

void Add2ConnectorCmd::sortIoLines
(
    vector<IoLine*>      & ioLines,
    MicroMeterLine  const& umLine
)
{
    MicroMeterLine const& orthoLine { umLine.OrthoLine() };
    sort
    (
        ioLines,
        [&orthoLine](auto p1, auto p2)
        {
            return PointToLine(orthoLine, p1->GetPos()) < PointToLine(orthoLine, p2->GetPos());
        }
    );
}
