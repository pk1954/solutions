// DiscConnCmd.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "MicroMeterPointVector.h"
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
    enum class tMode { remove, unplug, split };
    
    DiscConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const idConnector,
        tMode                const mode 
    )
      : m_idConnector(idConnector),
        m_mode(mode)
    {
        m_cmdStack.Initialize(&nmwi, nullptr);
        Connector & conn { * nmwi.GetNobPtr<Connector *>(m_idConnector) };

        switch (m_mode)
        {
        case tMode::remove:
            conn.Apply2All([&](Nob const &n) { m_cmdStack.Push(move(MakeDeleteCommand(nmwi, n))); });
            break;

        case tMode::unplug:
            break;

        case tMode::split :
            //            umPntVector.Pack( xxxxxxx );
            break;

        default:
            assert( false );
            break;
        }
    }

    ~DiscConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_idConnector);
        m_upConnector->ClearParentPointers();
        if (m_mode == tMode::remove)
            m_cmdStack.DoAll();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upConnector->SetParentPointers();
        nmwi.GetUPNobs().SetNob2Slot( move(m_upConnector) );
        if (m_mode == tMode::remove)
            m_cmdStack.UndoAll();
    }

private:

    NobId           const m_idConnector;
    tMode           const m_mode;
    CommandStack          m_cmdStack {};
    unique_ptr<Connector> m_upConnector {};  
};
