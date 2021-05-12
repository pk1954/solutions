// DiscConnCmd.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "DiscBaseKnotCmd.h"
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
        bool                 const bDelete 
    )
      : m_idConnector(idConnector),
        m_bDelete(bDelete)
    {
        //m_cmdStack.Initialize(&nmwi, nullptr);
        //if (m_bDelete)
        //{
        //    nmwi.GetNobPtr<Connector *>(m_idConnector)->Apply2All
        //    (
        //        [&](Nob const & s) 
        //        { 
        //            m_cmdStack.Push(move(make_unique<DiscBaseKnotCmd>(nmwi, s.GetId(), true)));
        //        }
        //    );
        //}
    }

    ~DiscConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_idConnector);
        m_upConnector->ClearParentPointers();
        //if (m_bDelete)
            //m_cmdStack.DoAll();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upConnector->SetParentPointers();
        nmwi.GetUPNobs().SetNob2Slot( move(m_upConnector) );
        //if (m_bDelete)
            //m_cmdStack.UndoAll();
    }

private:

    NobId           const m_idConnector;
    bool            const m_bDelete;     // true: delete Connector, false: disconnect only
    //CommandStack          m_cmdStack {};
    unique_ptr<Connector> m_upConnector {};  
};
