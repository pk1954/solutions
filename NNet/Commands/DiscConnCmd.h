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
#include "IoConnector.h"

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
      : m_connector(*Cast2IoConnector(&nob)),
        m_bRemove(bRemove)
    {
        m_cmdStack.Initialize(&nmwi, nullptr);
        if (m_bRemove)
            m_connector.Apply2All([&](Nob const &n) { m_cmdStack.Push(move(MakeDeleteCommand(nmwi, n))); });
    }

    ~DiscConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upIoConnector = nmwi.RemoveFromModel<IoConnector>(m_connector);
        m_upIoConnector->ClearParentPointers();
        if (m_bRemove)
            m_cmdStack.DoAll();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upIoConnector->SetParentPointers();
        nmwi.Restore2Model( move(m_upIoConnector) );
        if (m_bRemove)
            m_cmdStack.UndoAll();
    }

private:

    bool            const m_bRemove;
    CommandStack          m_cmdStack    {};
    unique_ptr<IoConnector> m_upIoConnector {};  
    IoConnector     const & m_connector;
};
