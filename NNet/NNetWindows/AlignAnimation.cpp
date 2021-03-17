// AlignAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "AlignAnimation.h"

void AlignAnimation::Initialize
( 
	HWND               const   hwnd,
	NNetModelWriterInterface & nmwi,
	NNetModelCommands        & commands
)
{
	m_pNMWI         = & nmwi;
	m_pNNetCommands = & commands;
	m_upConnectorAnimation = make_unique<ConnectorAnimation>(IDX_CONNECTOR_ANIMATION, hwnd); 
}

MicroMeterLine const AlignAnimation::calcMaxDistLine(ALIGN_VECTOR const & points)
{
	ConnectionNeuron const * pStart { nullptr };
	ConnectionNeuron const * pEnd   { nullptr };

	MicroMeter maxDist { 0.0_MicroMeter };   	// find 2 ConnectionNeurons (START/END) with maximum distance

	for ( auto & it1 : points )
	for ( auto & it2 : points )
	{
		MicroMeter dist { Distance( it1.pConnectionNeuron->GetPosition(), it2.pConnectionNeuron->GetPosition() ) };
		if ( dist > maxDist )
		{
			maxDist = dist;
			pStart = it1.pConnectionNeuron;
			pEnd   = it2.pConnectionNeuron;
		}
	}

	return MicroMeterLine( pStart->GetPosition(), pEnd->GetPosition());
}

MicroMeterLine const AlignAnimation::prepareData
( 
	ShapePtrList<ConnectionNeuron> & shapesAnimated,
	UPShapeList                    & shapes
)
{
	ShapeType const shapeType { shapes.DetermineShapeType() };

	if ( shapeType.IsUndefinedType() )
		return MicroMeterLine::NULL_VAL();

	ALIGN_VECTOR points;

	shapes.Apply2AllSelected
	( 
		shapeType,
		[&](Shape & s)
		{ 
			points.push_back( ALIGN_PNT { static_cast<ConnectionNeuron *>(&s) } );
		} 
	);

	MicroMeterLine line      { calcMaxDistLine( points ) };
	MicroMeterLine orthoLine { line.OrthoLine() };

	for ( auto & it : points )
		it.umDist = PointToLine( orthoLine, it.pConnectionNeuron->GetPosition() );

	// sort ConnectionNeurons according to position related to ortholine

	sort( points.begin(), points.end(), [](auto & p1, auto & p2) { return p1.umDist < p2.umDist; } );

	shapesAnimated.Clear();
	for ( auto & it : points )
		shapesAnimated.Add( it.pConnectionNeuron );

	return line;
}

MicroMeterPoint const AlignAnimation::calcOrthoVector
(
	ShapePtrList<ConnectionNeuron> const & list,
	MicroMeterLine                 const & line
)
{
	unsigned int uiLeftConnections  { 0 };
	unsigned int uiRightConnections { 0 };
	list.Apply2All
	(	
		[&](ConnectionNeuron const & c)	
		{ 
			c.m_connections.Apply2AllInPipes
			( 
				[&](Pipe & pipe) 
				{ 
					MicroMeterPoint pnt { pipe.GetStartPoint() };
					if ( PointToLine( line, pnt ) < 0.0_MicroMeter )
						++uiLeftConnections;
					else
						++uiRightConnections;
				}
			);
			c.m_connections.Apply2AllOutPipes
			( 
				[&](Pipe & pipe) 
				{ 
					MicroMeterPoint pnt { pipe.GetEndPoint() };
					if ( PointToLine( line, pnt ) < 0.0_MicroMeter )
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

bool AlignAnimation::AlignSelection( )
{
	MicroMeterLine  const line        { prepareData( m_shapesAnimated, m_pNMWI->GetUPShapes() ) };
	MicroMeterPoint const orthoVector { calcOrthoVector(m_shapesAnimated, line) };

	// compute tightly packed positions

	MicroMeterPoint umPntPackedSingle { line.GetVector() * (NEURON_RADIUS * 2.0f / line.Length()) };
	MicroMeterPoint umPntTargetStart  { line.GetCenter() - umPntPackedSingle * Cast2Float(m_shapesAnimated.Size()) * 0.5f };

	// fill animation vectors

	MicroMeterPosDir      posDirTarget { umPntTargetStart, Vector2Radian(orthoVector) };
	MicroMeterPointVector umPntVectorActual;
	MicroMeterPointVector umPntVectorTarget;

	wcout << L"target:" << endl;
	m_shapesAnimated.Apply2All
	(	
		[&](ConnectionNeuron const & c)	
		{ 
			umPntVectorActual.Add( c.GetPosDir() );
			umPntVectorTarget.Add( posDirTarget );
			wcout << __func__ << posDirTarget << endl;
			posDirTarget += umPntPackedSingle;
		}	
	);

	m_upConnectorAnimation->Start( umPntVectorActual, umPntVectorTarget );
	return true;
}

void AlignAnimation::AnimationStep( )
{
	m_pNNetCommands->SetConnectionNeurons
	(
		m_upConnectorAnimation->GetActual(), 
		m_shapesAnimated
	);
}
