// DeleteClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "DiscConnCmd.h"
#include "Command.h"
#include "NobId.h"
#include "ClosedConnector.h"

using std::unique_ptr;

class DeleteClosedConnCmd : public Command
{
public:
    DeleteClosedConnCmd
    (
        NNetModelWriterInterface & nmwi,
        Nob                      & nob
    )
      : m_closedIoConnector(*Cast2ClosedConnector(&nob))
    {
    }

    ~DeleteClosedConnCmd() {}

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_closedIoConnector);
        m_upClosedConnector->ClearParentPointers();
    }

    virtual void Undo(NNetModelWriterInterface & nmwi)
    {
        m_upClosedConnector->SetParentPointers();
        nmwi.Restore2Model(move(m_upClosedConnector));
    }

private:

    ClosedConnector           & m_closedIoConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};
};
