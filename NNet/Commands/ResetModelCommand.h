// ResetModelCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "Command.h"

class ResetModelCommand : public Command
{
public:
	ResetModelCommand( NNetModel * const pModel )
	{
		m_pModel = new NNetModel( * pModel ); // deep copy
	}

	~ResetModelCommand( )
	{
		delete m_pModel;
	}

	virtual void Do( NNetModel * const pModel )
	{ 
		pModel->ResetModel( );
		pModel->CreateInitialShapes();
	}

	virtual void Undo( NNetModel * const pModel ) 
	{
		pModel->ResetModel( );
		* pModel = * m_pModel; // shallow assignment
	}

private:
	NNetModel const * m_pModel;
};

