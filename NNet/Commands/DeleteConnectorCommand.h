// DeleteConnectorCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "ShapeId.h"
#include "Connector.h"

using std::unique_ptr;

class DeleteConnectorCommand : public Command
{
public:
    DeleteConnectorCommand(ShapeId const idShape)
        : m_idShape(idShape)
    {}

    ~DeleteConnectorCommand() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upConnector = nmwi.RemoveFromModel<Connector>(m_idShape);
        m_upConnector->ClearParentPointers();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upConnector->SetParentPointers();
        nmwi.GetUPShapes().SetShape2Slot( move(m_upConnector) );
    }

private:

    ShapeId         const m_idShape;
    unique_ptr<Connector> m_upConnector {};  
};
