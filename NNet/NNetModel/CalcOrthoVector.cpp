// CalcOrthoVector.cpp
//
// NNetModel

#include "stdafx.h"
#include "Pipe.h"
#include "CalcOrthoVector.h"

MicroMeterPoint const CalcOrthoVector
(
	MicroMeterLine         const & line,
	ShapePtrList<BaseKnot> const & list
)
{
	unsigned int uiLeftConnections  { 0 };
	unsigned int uiRightConnections { 0 };
	list.Apply2All
	(	
		[&](BaseKnot const & baseKnot)
		{ 
			baseKnot.m_connections.Apply2AllInPipes
			( 
				[&](Pipe & pipe) 
				{ 
					MicroMeterPoint pnt { pipe.GetStartPoint() };
					if ( PointToLine(line, pnt) < 0.0_MicroMeter )
						++uiLeftConnections;
					else
						++uiRightConnections;
				}
			);
			baseKnot.m_connections.Apply2AllOutPipes
			( 
				[&](Pipe & pipe) 
				{ 
					MicroMeterPoint pnt { pipe.GetEndPoint() };
					if ( PointToLine(line, pnt) < 0.0_MicroMeter )
						++uiRightConnections;
					else
						++uiLeftConnections;
				}
			);
		}	
	);

	MicroMeterPoint orthoVector = line.OrthoVector();
	if ( uiRightConnections < uiLeftConnections )
		orthoVector = -orthoVector;
	return orthoVector;
}

