// AnalyzeLoopsCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Analyzer.h"
#include "SelectionCommand.h"

class AnalyzeLoopsCommand : public SelectionCommand
{
public:
	AnalyzeLoopsCommand( NNetModelWriterInterface * const pModel )
		:	SelectionCommand( pModel)
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->SelectAllShapes( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindLoop( pModel->GetModel() ) )
			ModelAnalyzer::SelectLoopShapes( * pModel );
	}
};
