// DiscClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "DiscConnCmd.h"
#include "Command.h"
#include "NobId.h"
#include "ClosedConnector.h"

using std::unique_ptr;

class DiscClosedConnCmd : public Command
{
public:
    DiscClosedConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const idClosedConnector,
        bool                 const bDelete 
    )
      : m_idClosedConnector(idClosedConnector),
        m_bDelete(bDelete)
    {
        ClosedConnector const & closedConnector { * nmwi.GetNobPtr<ClosedConnector *>(idClosedConnector) };
        m_upInputConnectorCmd  = move(make_unique<DiscConnCmd>(nmwi, closedConnector.GetInputconnector ().GetId(), DiscConnCmd::tMode::remove));
        m_upOutputConnectorCmd = move(make_unique<DiscConnCmd>(nmwi, closedConnector.GetOutputconnector().GetId(), DiscConnCmd::tMode::remove));
    }

    ~DiscClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_idClosedConnector);
        m_upClosedConnector->ClearParentPointers();
        if (m_bDelete)
        {
            m_upInputConnectorCmd ->Do(nmwi);
            m_upOutputConnectorCmd->Do(nmwi);
        }
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector->SetParentPointers();
        nmwi.GetUPNobs().SetNob2Slot( move(m_upClosedConnector) );
        if ( m_bDelete )
        {
            m_upOutputConnectorCmd->Undo(nmwi);
            m_upInputConnectorCmd ->Undo(nmwi);
        }
    }

private:

    bool                  const m_bDelete;     // true: delete Connector, false: disconnect only
    NobId                 const m_idClosedConnector;
    unique_ptr<DiscConnCmd>     m_upInputConnectorCmd  {};  
    unique_ptr<DiscConnCmd>     m_upOutputConnectorCmd {};  
    unique_ptr<ClosedConnector> m_upClosedConnector    {};  
};
