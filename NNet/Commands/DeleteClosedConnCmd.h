// DeleteClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "DiscIoConnectorCmd.h"
#include "Command.h"
#include "NobId.h"
#include "ClosedConnector.h"

using std::unique_ptr;

class DeleteClosedConnCmd : public Command
{
public:
    DeleteClosedConnCmd(Nob & nob)
      : m_closedConnector(Cast2ClosedConnector(nob))
    {}

    ~DeleteClosedConnCmd() {}

    virtual void Do()
    {
        m_upClosedConnector = m_pNMWI->RemoveFromModel<ClosedConnector>(m_closedConnector);
        m_upClosedConnector->ClearParentPointers();
    }

    virtual void Undo()
    {
        m_upClosedConnector->SetParentPointers();
        m_pNMWI->Restore2Model<ClosedConnector>(move(m_upClosedConnector));
    }

private:

    ClosedConnector           & m_closedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};
};
