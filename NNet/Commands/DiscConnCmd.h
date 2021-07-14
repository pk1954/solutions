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
        Nob                      & nob,
        bool                 const bRemove 
    )
      : m_connector(*Cast2Connector(&nob)),
        m_bRemove(bRemove)
    {
        m_cmdStack.Initialize(&nmwi, nullptr);
        if (m_bRemove)
            m_connector.Apply2All([&](Nob const &n) { m_cmdStack.Push(move(MakeDeleteCommand(nmwi, n))); });
    }

    ~DiscConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_connector);
        m_upConnector->ClearParentPointers();
        if (m_bRemove)
            m_cmdStack.DoAll();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upConnector->SetParentPointers();
        nmwi.Restore2Model( move(m_upConnector) );
        if (m_bRemove)
            m_cmdStack.UndoAll();
    }

private:

    bool            const m_bRemove;
    CommandStack          m_cmdStack    {};
    unique_ptr<Connector> m_upConnector {};  
    Connector     const & m_connector;
};
