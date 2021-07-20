// SplitClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "NobId.h"
#include "ClosedConnector.h"

using std::unique_ptr;

class SplitClosedConnCmd : public Command
{
public:
    SplitClosedConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const id
    )
      : m_closedConnector(*nmwi.GetNobPtr<ClosedConnector *>(id))
    {
        m_upInputConnector  = make_unique<Connector>(NobIoMode::input);
        m_upOutputConnector = make_unique<Connector>(NobIoMode::output);
        for (Neuron * pNeuron : m_closedConnector.GetNeurons())
        {
            unique_ptr<InputNeuron > upInputNeuron  { make_unique<InputNeuron >(*pNeuron) };
            unique_ptr<OutputNeuron> upOutputNeuron { make_unique<OutputNeuron>(*pNeuron) };
            m_upInputConnector ->Push(upInputNeuron .get());
            m_upOutputConnector->Push(upOutputNeuron.get());
        }
    }

    ~SplitClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_closedConnector);
        m_upClosedConnector->ClearParentPointers();
        m_upInputConnector ->SetParentPointers();
        m_upOutputConnector->SetParentPointers();
        nmwi.Push2Model(move(m_upInputConnector));
        nmwi.Push2Model(move(m_upOutputConnector));
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upOutputConnector = nmwi.PopFromModel<Connector>();
        m_upInputConnector  = nmwi.PopFromModel<Connector>();
        m_upClosedConnector->SetParentPointers();
        nmwi.Restore2Model(move(m_upClosedConnector));
    }

private:

    ClosedConnector           & m_closedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};
    unique_ptr<Connector>       m_upInputConnector  {};  
    unique_ptr<Connector>       m_upOutputConnector {};  
};
