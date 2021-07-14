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
      : m_closedConnector(*Cast2ClosedConnector(&nob))
    {
  //      m_upDiscInputConn = make_unique<DiscConnCmd>(nmwi, )
    }

    ~DeleteClosedConnCmd() {}

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_closedConnector);
        m_upClosedConnector->ClearParentPointers();
    }

    virtual void Undo(NNetModelWriterInterface & nmwi)
    {
        m_upClosedConnector->SetParentPointers();
        nmwi.Restore2Model(move(m_upClosedConnector));
    }

private:

    ClosedConnector           & m_closedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};
    unique_ptr<DiscConnCmd>     m_upDiscInputConn   {};
    unique_ptr<DiscConnCmd>     m_upDiscOutputConn  {};
};
