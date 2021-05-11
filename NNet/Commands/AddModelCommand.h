// AddModelCommand.h
//
// Commands

#pragma once

#include "UPNobList.h"
#include "NobIdList.h"
#include "SelectionCommand.h"
#include "NNetModelWriterInterface.h"

class AddModelCommand : public SelectionCommand
{
public:

	AddModelCommand( UPNobList const & list )
	{ 
		m_UPNobList = list;
		m_UPNobList.SelectAllNobs(true);
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do(nmwi);
		nmwi.GetUPNobs().SelectAllNobs(false);
		m_idList = nmwi.GetUPNobs().Append( m_UPNobList );
	}

	virtual void Undo( NNetModelWriterInterface  & nmwi ) 
	{ 
		m_UPNobList = nmwi.GetUPNobs().ExtractNobs( m_idList );
		SelectionCommand::Undo( nmwi );
	}

private:
	UPNobList m_UPNobList;
	NobIdList m_idList;
};
