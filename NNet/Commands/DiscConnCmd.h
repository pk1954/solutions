// DiscConnCmd.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"
#include "DiscBaseKnotCmd.h"
#include "CommandFunctions.h"
#include "Command.h"
#include "CommandStack.h"
#include "NobId.h"
#include "UpNobList.h"
#include "Connector.h"

using std::unique_ptr;

class DiscConnCmd : public Command
{
public:
    DiscConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const idConnector,
        bool                 const bRemove 
    )
      : m_idConnector(idConnector),
        m_bRemove(bRemove)
    {
        m_cmdStack.Initialize(&nmwi, nullptr);
        Connector & conn { * nmwi.GetNobPtr<Connector *>(m_idConnector) };

        if (m_bRemove)
            conn.Apply2All([&](Nob const &n) { m_cmdStack.Push(move(MakeDeleteCommand(nmwi, n))); });
    }

    ~DiscConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_idConnector);
        m_upConnector->ClearParentPointers();
        if (m_bRemove)
            m_cmdStack.DoAll();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upConnector->SetParentPointers();
        nmwi.GetUPNobs().SetNob2Slot( move(m_upConnector) );
        if (m_bRemove)
            m_cmdStack.UndoAll();
    }

private:

    NobId           const m_idConnector;
    bool            const m_bRemove;
    CommandStack          m_cmdStack {};
    unique_ptr<Connector> m_upConnector {};  
};
