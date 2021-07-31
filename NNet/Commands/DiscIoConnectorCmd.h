// DiscIoConnectorCmd.h
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

class DiscIoConnectorCmd : public Command
{
public:
    DiscIoConnectorCmd
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
            m_connector.Apply2All
            (
                [&](IoNeuron & n) 
                { 
                    m_cmdStack.Push(move(MakeDeleteCommand(nmwi, n))); 
                }
            );
    }

    ~DiscIoConnectorCmd() {}

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
        nmwi.Restore2Model<IoConnector>(move(m_upIoConnector));
        if (m_bRemove)
            m_cmdStack.UndoAll();
    }

private:

    bool              const m_bRemove;
    CommandStack            m_cmdStack    {};
    unique_ptr<IoConnector> m_upIoConnector {};  
    IoConnector     const & m_connector;
};
