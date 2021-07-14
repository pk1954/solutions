// UnplugClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "DiscConnCmd.h"
#include "Command.h"
#include "NobId.h"
#include "ClosedConnector.h"

using std::unique_ptr;

class UnplugClosedConnCmd : public Command
{
public:
    UnplugClosedConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const idClosedConnector,
        bool                 const bDelete 
    )
      : m_idClosedConnector(idClosedConnector),
        m_bDelete(bDelete)
    {
        ClosedConnector const & closedConnector { * nmwi.GetNobPtr<ClosedConnector *>(idClosedConnector) };
        m_upInputConnector  = make_unique<Connector>(closedConnector.GetInputNeurons ());
        m_upOutputConnector = make_unique<Connector>(closedConnector.GetOutputNeurons());
    }

    ~UnplugClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_idClosedConnector);
        m_upClosedConnector->ClearParentPointers();
        if (!m_bDelete)
        {
            m_upInputConnector ->SetParentPointers();
            m_upOutputConnector->SetParentPointers();
            nmwi.Push2Model(move(m_upInputConnector));
            nmwi.Push2Model(move(m_upOutputConnector));
        }
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector->SetParentPointers();
        nmwi.Restore2Model(move(m_upClosedConnector));
        if (!m_bDelete)
        {
            m_upOutputConnector = nmwi.PopFromModel<Connector>();
            m_upInputConnector  = nmwi.PopFromModel<Connector>();
        }
    }

private:

    bool                  const m_bDelete;     // true: delete Connector, false: disconnect only
    NobId                 const m_idClosedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector    {};
    unique_ptr<Connector>       m_upInputConnector     {};  
    unique_ptr<Connector>       m_upOutputConnector    {};  
};
