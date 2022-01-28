// DiscIoConnectorCmd.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"
#include "DiscBaseKnotCmd.h"
#include "CommandFunctions.h"
#include "NNetCommand.h"
#include "CommandStack.h"
#include "NobId.h"
#include "UPNobList.h"
#include "IoConnector.h"

using std::unique_ptr;

class DiscIoConnectorCmd : public NNetCommand
{
public:
    DiscIoConnectorCmd
    (
        Nob      & nob,
        bool const bRemove 
    )
      : m_connector(*Cast2IoConnector(&nob)),
        m_bRemove(bRemove)
    {
        m_cmdStack.Initialize(m_pNMWI, nullptr);
        if (m_bRemove)
            m_connector.Apply2All
            (
                [this](Neuron & n) 
                { 
                    if (unique_ptr<NNetCommand> upCmd { MakeDeleteCommand(*m_pNMWI, n) })
                        m_cmdStack.Push(move(upCmd)); 
                }
           );
    }

    void Do() final
    {
        m_upIoConnector = m_pNMWI->RemoveFromModel<IoConnector>(m_connector);
        m_upIoConnector->ClearParentPointers();
        if (m_bRemove)
            m_cmdStack.DoAll();
    }

    void Undo() final
    {
        m_upIoConnector->SetParentPointers();
        m_pNMWI->Restore2Model<IoConnector>(move(m_upIoConnector));
        if (m_bRemove)
            m_cmdStack.UndoAll();
    }

private:

    IoConnector     const & m_connector;
    bool            const   m_bRemove;
    CommandStack            m_cmdStack      {};
    unique_ptr<IoConnector> m_upIoConnector {};  
};
