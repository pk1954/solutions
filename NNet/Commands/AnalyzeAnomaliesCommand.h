// AnalyzeAnomaliesCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Analyzer.h"
#include "SelectionCommand.h"

class AnalyzeAnomaliesCommand : public SelectionCommand
{
public:
	AnalyzeAnomaliesCommand( NNetModelWriterInterface & model )
		:	SelectionCommand( model)
	{ }

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.SelectAllShapes( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindAnomaly( model ) )
			ModelAnalyzer::SelectLoopShapes( model );
	}
};
