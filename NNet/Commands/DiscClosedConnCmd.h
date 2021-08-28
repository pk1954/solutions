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
    DiscClosedConnCmd(NobId const idClosedConnector)
        : m_idClosedConnector(idClosedConnector)
    {}

    ~DiscClosedConnCmd() {}

    virtual void Do()
    {
        m_upClosedConnector = m_pNMWI->RemoveFromModel<ClosedConnector>(m_idClosedConnector); // Take ownership of ClosedConnector
        m_upClosedConnector->ClearParentPointers();
    }

    virtual void Undo()
    { 
        m_upClosedConnector->SetParentPointers();
        m_pNMWI->Restore2Model<ClosedConnector>(move(m_upClosedConnector)); // Move ownership of ClosedConnector to model
    }

private:

    unique_ptr<ClosedConnector> m_upClosedConnector {};
    NobId                const  m_idClosedConnector;
};
