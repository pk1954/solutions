// CreateConnectorCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "UPShapeList.h"
#include "ShapePtrList.h"
#include "Command.h"
#include "Connector.h"
#include "ConnectionNeuron.h"

using std::unique_ptr;
using std::make_unique;

class CreateConnectorCommand : public Command
{
public:
	CreateConnectorCommand(ShapePtrList<ConnectionNeuron> & list)
	  : m_shapes4connector(list)
	{
		m_upConnector = make_unique<Connector>();
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shapes4connector.Apply2All
		(
			[&](ConnectionNeuron & n)
			{
				m_upConnector->Push(move(nmwi.RemoveFromModel<ConnectionNeuron>(n)));
			}
		);
		m_idConnector = nmwi.GetUPShapes().Push( move(m_upConnector) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_upConnector = nmwi.RemoveFromModel<Connector>(* nmwi.GetShapePtr<Connector *>(m_idConnector));
		while (m_upConnector->IsNotEmpty())
			nmwi.GetUPShapes().SetShape2Slot(move(m_upConnector->Pop()));
	}

private:

	ShapePtrList<ConnectionNeuron> m_shapes4connector;
	unique_ptr  <Connector>        m_upConnector {};
	ShapeId                        m_idConnector {};
};
