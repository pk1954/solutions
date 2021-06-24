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
      : m_pConnector(nmwi.GetNobPtr<Connector *>(idConnector)),
        m_bRemove(bRemove)
    {
        initialize(nmwi);
    }

    //DiscConnCmd
    //(
    //    NNetModelWriterInterface & nmwi,
    //    unique_ptr<Connector>      conn,
    //    bool               const   bRemove 
    //)
    //  : m_upConnector(move(conn)),
    //    m_bRemove(bRemove)
    //{
    //    initialize(nmwi);
    //}

    ~DiscConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_pConnector->GetId());
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
    void initialize(NNetModelWriterInterface & nmwi)
    {
        m_cmdStack.Initialize(&nmwi, nullptr);
        if (m_bRemove)
            m_pConnector->Apply2All([&](Nob const &n) { m_cmdStack.Push(move(MakeDeleteCommand(nmwi, n))); });
    }

    bool            const m_bRemove;
    CommandStack          m_cmdStack    {};
    unique_ptr<Connector> m_upConnector {};  
    Connector     const * m_pConnector  { nullptr };
};
