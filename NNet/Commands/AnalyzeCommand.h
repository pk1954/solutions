// AnalyzeLoopsCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Analyzer.h"
#include "SelectionCommand.h"

class AnalyzeCommand : public SelectionCommand
{
public:
	AnalyzeCommand( ShapeStack const stack )
		: m_result ( stack )
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		if ( ! m_result.IsEmpty() )
		{
			nmwi.GetUPShapes().DeselectAllShapes();
			m_result.Apply2All([&](Shape const &s){ nmwi.SelectShape(s.GetId()); }	);
		}
	}

protected:
	ShapeStack m_result;
};
