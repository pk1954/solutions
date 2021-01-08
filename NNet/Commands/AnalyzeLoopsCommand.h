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
	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.GetShapes().SelectAllShapes( tBoolOp::opFalse );
		if ( ModelAnalyzer::FindLoop( nmwi ) )
			ModelAnalyzer::SelectLoopShapes( nmwi );
	}
};
