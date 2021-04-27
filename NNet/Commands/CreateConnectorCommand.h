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
#include "SelectionCommand.h"
#include "ConnectionNeuron.h"

using std::unique_ptr;
using std::make_unique;

class RootWindow;

class CreateConnectorCommand : public SelectionCommand
{
public:
	CreateConnectorCommand
	(
		unique_ptr<ShapePtrList<ConnNeuron>> upList,
		function<void(bool const)>   const & finFunc
	)
		: m_finFunc(finFunc)
	{
		m_upConnector = make_unique<Connector>();
		upList->Apply2All([&](ConnNeuron & n) { m_upConnector->Push(&n); });
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		m_upConnector->SetParentPointers();
		m_upConnector->Select(false, true);
		nmwi.GetUPShapes().Push( move(m_upConnector) );
		(m_finFunc)(true);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_upConnector = nmwi.GetUPShapes().Pop<Connector>();
		m_upConnector->ClearParentPointers();
		(m_finFunc)(false);
		SelectionCommand::Undo( nmwi );
	}

private:

	function<void(bool const)> const m_finFunc;
	unique_ptr<Connector>                        m_upConnector {};  
};
