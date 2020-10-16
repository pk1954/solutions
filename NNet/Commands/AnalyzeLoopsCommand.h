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
	AnalyzeLoopsCommand( NNetModelWriterInterface & model )
		:	SelectionCommand( model)
	{ }

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.SelectAllShapes( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindLoop( model ) )
			ModelAnalyzer::SelectLoopShapes( model );
	}
};
