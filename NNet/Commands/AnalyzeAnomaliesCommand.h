// AnalyzeAnomaliesCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "Analyzer.h"
#include "SelectionCommand.h"

class AnalyzeAnomaliesCommand : public SelectionCommand
{
public:
	AnalyzeAnomaliesCommand( NNetModel * const pModel )
		:	SelectionCommand( pModel)
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->SelectAll( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindAnomaly( * pModel ) )
			ModelAnalyzer::SelectLoopShapes( * pModel );
	}
};
