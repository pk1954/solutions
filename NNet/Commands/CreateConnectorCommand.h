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

class CreateConnectorCommand : public SelectionCommand
{
public:
	CreateConnectorCommand(ShapePtrList<ConnNeuron> & list)
	{
		m_upConnector = make_unique<Connector>();
		//m_upConnector->Select(true, false);
		list.Apply2All(	[&](ConnNeuron & n)	{ m_upConnector->Push(&n); } );
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upConnector->SetParentPointers();
		m_upConnector->Select(false, true);
		nmwi.GetUPShapes().Push( move(m_upConnector) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_upConnector = nmwi.GetUPShapes().Pop<Connector>();
		m_upConnector->ClearParentPointers();
	}

private:

	unique_ptr<Connector> m_upConnector {};  
};
