// AlignAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "CalcOrthoVector.h"
#include "AlignAnimation.h"

void AlignAnimation::Initialize
( 
	HWND               const   hwnd,
	NNetModelWriterInterface & nmwi,
	NNetModelCommands        & commands
)
{
	m_pNMWI           = & nmwi;
	m_pModelCommands  = & commands;
	m_upConnAnimation = make_unique<ConnAnimation>(IDX_CONNECTOR_ANIMATION, hwnd); 
}

bool const AlignAnimation::calcMaxDistLine(ALIGN_VECTOR const & points)
{
	MicroMeter maxDist { 0.0_MicroMeter };   	// find 2 ConnectionNeurons (START/END) with maximum distance
	m_line = MicroMeterLine::NULL_VAL();
	for ( auto & it1 : points )
	for ( auto & it2 : points )
	{
		auto const line { MicroMeterLine( it1.pConnectionNeuron->GetPosition(), it2.pConnectionNeuron->GetPosition() ) };
		auto const dist { line.Length() };
		if ( dist > maxDist )
		{
			maxDist = dist;
			m_line = line;
		}
	}
	return m_line.IsNotNull();
}

void AlignAnimation::calcDistances( ALIGN_VECTOR & points)
{
	MicroMeterLine orthoLine { m_line.OrthoLine() };
	for ( auto & it : points )
		it.umDist = PointToLine( orthoLine, it.pConnectionNeuron->GetPosition() );
}

void AlignAnimation::sortDistances( ALIGN_VECTOR & points) // sort ConnectionNeurons according to 
{                                                          // position related to ortholine
	sort
	( 
		points.begin(), points.end(),                              // all elements
		[](auto & p1, auto & p2) { return p1.umDist < p2.umDist; } // sort criterion
	);
}

bool const AlignAnimation::prepareData()
{
	ShapeType const shapeType { m_pNMWI->GetUPShapes().DetermineShapeType() };

	if ( shapeType.IsUndefinedType() )
	{
		m_line = MicroMeterLine::NULL_VAL();
		return false;
	}

	ALIGN_VECTOR points;

	m_pNMWI->GetUPShapes().Apply2AllSelected
	( 
		shapeType,
		[&points](Shape & s) { points.push_back( ALIGN_PNT { static_cast<ConnNeuron *>(&s) } ); } 
	);

	if ( ! calcMaxDistLine( points ) )
		return false;

	calcDistances( points );
	sortDistances( points );

	m_shapesAnimated.Clear();
	for ( auto & it : points )
		m_shapesAnimated.Add( it.pConnectionNeuron );

	return true;
}

bool const AlignAnimation::AlignSelection( AlignAnimation::Script const & script )
{
	m_pScript = & script;
	m_iScriptStep = 0;

	if ( ! prepareData() )
		return false;

	scriptStep( m_pScript->at(m_iScriptStep) );
	return true;
}

void AlignAnimation::scriptStep(DWORD const dwOptions)
{
	bool bAlignDirection  = dwOptions & ALIGN_DIRECTION;
	bool bAlignShapes     = dwOptions & (ALIGN_SHAPES|PACK_SHAPES);
	bool bPackShapes      = dwOptions & PACK_SHAPES;

	m_orthoVector = CalcOrthoVector(m_line, m_shapesAnimated);

	float      const fGapCount          { Cast2Float(m_shapesAnimated.Size() - 1) };
	MicroMeter const umUnpackedDistance { m_line.Length() / fGapCount };
	MicroMeter const umShapeDistTarget  { bPackShapes ? NEURON_RADIUS * 2.0f : umUnpackedDistance };
	MicroMeter const umLineLengthTarget { umShapeDistTarget * fGapCount };

	// compute target positions (packed if PACK_SHAPES is active)

	MicroMeterPoint umPntSingleVector { m_line.GetVector().ScaledTo(umShapeDistTarget) };
	MicroMeterPoint umPntLineTarget   { m_line.GetVector().ScaledTo(umLineLengthTarget) };
	MicroMeterPoint umPntTargetStart  { m_line.GetCenter() - umPntLineTarget * 0.5f };

	// fill animation vectors

	MicroMeterPosDir      posDirTarget { umPntTargetStart, Vector2Radian(m_orthoVector) };
	MicroMeterPointVector umPntVectorTarget {};

	m_shapesAnimated.Apply2All
	(	
		[&](ConnNeuron const & c)	
		{ 
			MicroMeterPosDir posDir 
			{
				bAlignShapes    ? posDirTarget.GetPos() : c.GetPosition(), 
				bAlignDirection ? posDirTarget.GetDir() : Vector2Radian(c.GetDirVector()) 
			};
			umPntVectorTarget.Add( posDir );
			posDirTarget += umPntSingleVector;
		}	
	);

	m_upConnAnimation->Start( MicroMeterPointVector(m_shapesAnimated), umPntVectorTarget );
}

bool const AlignAnimation::AnimationStep(bool const bTargetReached)
{
	m_pModelCommands->SetConnectionNeurons
	(
		m_upConnAnimation->GetActual(), 
		m_shapesAnimated
	);

	if ( bTargetReached )
	{
		if (++m_iScriptStep == m_pScript->size())
			return true;
		DWORD dwOptions { m_pScript->at(m_iScriptStep) };
		if (dwOptions & CREATE_CONNECTOR)
		{
			assert(m_iScriptStep == m_pScript->size() - 1);  // assumption: Create Connector is last step
			m_pModelCommands->CreateConnector( m_shapesAnimated );
			return true;
		}
		else
		{
			scriptStep( dwOptions );
		}
	}
	return false;
}
