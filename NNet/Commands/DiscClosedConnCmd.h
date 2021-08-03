// DiscClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
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
        NobId                const idClosedConnector
   )
        : m_idClosedConnector(idClosedConnector)
    {}

    ~DiscClosedConnCmd() {}

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_idClosedConnector); // Take ownership of ClosedConnector
        m_upClosedConnector->ClearParentPointers();
    }

    virtual void Undo(NNetModelWriterInterface & nmwi)
    { 
        m_upClosedConnector->SetParentPointers();
        nmwi.Restore2Model<ClosedConnector>(move(m_upClosedConnector)); // Move ownership of ClosedConnector to model
    }

private:

    unique_ptr<ClosedConnector> m_upClosedConnector {};
    NobId                const  m_idClosedConnector;
};
