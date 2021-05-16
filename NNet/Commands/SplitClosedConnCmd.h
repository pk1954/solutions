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
        NobId                const idClosedConnector
    )
        : m_idClosedConnector(idClosedConnector)
    {
        ClosedConnector const & closedConnector { * nmwi.GetNobPtr<ClosedConnector *>(idClosedConnector) };
    }

    ~SplitClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_idClosedConnector);
        m_upClosedConnector->ClearParentPointers();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector->SetParentPointers();
        nmwi.GetUPNobs().SetNob2Slot( move(m_upClosedConnector) );
    }

private:

    NobId                 const m_idClosedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};  
};
