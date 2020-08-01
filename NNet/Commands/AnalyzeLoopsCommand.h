// AnalyzeLoopsCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "Analyzer.h"
#include "SelectionCommand.h"

class AnalyzeLoopsCommand : public SelectionCommand
{
public:
	AnalyzeLoopsCommand( NNetModel * const pModel )
		:	SelectionCommand( pModel)
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->SelectAll( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindLoop( * pModel ) )
			ModelAnalyzer::SelectLoopShapes( * pModel );
	}
};
