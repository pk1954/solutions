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
	AnalyzeAnomaliesCommand( NNetModelWriterInterface * const pModel )
		:	SelectionCommand( pModel)
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->SelectAllShapes( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindAnomaly( pModel->GetModel() ) )
			ModelAnalyzer::SelectLoopShapes( * pModel );
	}
};
