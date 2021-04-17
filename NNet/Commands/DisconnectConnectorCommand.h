// DisconnectConnectorCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "ShapeId.h"
#include "UpShapeList.h"
#include "Connector.h"

using std::unique_ptr;

class DisconnectConnectorCommand : public Command
{
public:
    DisconnectConnectorCommand
    (
        ShapeId const idConnector,
        bool    const bDelete 
    )
      : m_idConnector(idConnector),
        m_bDelete(bDelete)
    {
    }

    ~DisconnectConnectorCommand() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_idConnector);
        m_upConnector->ClearParentPointers();
        if (m_bDelete)
        {
            ShapeIdList(*m_upConnector.get()).Apply2All
            (
                [&](ShapeId const & id) 
                { 
                    m_upConnNeurons.push_back(move(nmwi.RemoveFromModel<ConnNeuron>(id)));
                }
            );
        }
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upConnector->SetParentPointers();
        nmwi.GetUPShapes().SetShape2Slot( move(m_upConnector) );
        if (m_bDelete)
        {
            while (!m_upConnNeurons.empty())
            {
                auto upConnNeuron { move(m_upConnNeurons.back().release()) };
                nmwi.ReplaceInModel<ConnNeuron,ConnNeuron>( move(upConnNeuron) ); 
                m_upConnNeurons.pop_back();
           }
        }
    }

private:

    ShapeId                  const m_idConnector;
    vector<unique_ptr<ConnNeuron>> m_upConnNeurons;  // intentionally no UPShapeList!
    unique_ptr<Connector>          m_upConnector {};  
    bool                           m_bDelete;        // true: delete Connector, false: disconnect only
};
