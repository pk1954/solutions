// AlignAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelCommands.h"
#include "Neuron.h"
#include "AlignAnimation.h"

AlignAnimation::AlignAnimation
( 
	NNetModelReaderInterface const & nmri,
	NNetModelCommands              & commands,
	HWND                     const   hwnd,
	int                      const   idMsg
)
  :	m_pNMRI(&nmri),
	m_pNNetCommands(&commands)
{
	m_upConnectorAnimation = make_unique<Animation<MicroMeterPointVector>>(idMsg, hwnd); 
}

ShapeType const AlignAnimation::determineShapeType()
{
	int iNrOfInputNeurons  { m_pNMRI->GetUPShapes().CountInSelection( ShapeType::Value::inputNeuron  ) };
	int iNrOfOutputNeurons { m_pNMRI->GetUPShapes().CountInSelection( ShapeType::Value::outputNeuron ) };
	return ((iNrOfInputNeurons == 0) && (iNrOfOutputNeurons == 0))
           ? ShapeType::Value::undefined
		   : (iNrOfInputNeurons > iNrOfOutputNeurons) 
			  ? ShapeType::Value::inputNeuron 
			  : ShapeType::Value::outputNeuron;
}

bool AlignAnimation::AlignSelection( )
{
	struct AL_PNT
	{
		BaseKnot * pBaseKnot;
		MicroMeter umDist;
	};

	vector<AL_PNT> points;
	ShapeType const shapeType { determineShapeType() };

	if ( shapeType.IsUndefinedType() )
		return false;

	m_pNMRI->GetUPShapes().Apply2AllSelected<BaseKnot>
	( 
		[&](BaseKnot const & b)
		{ 
			if ( b.GetShapeType() == shapeType )  // TODO: const_cast find better solution
			{
				BaseKnot * pBaseKnot { const_cast<BaseKnot *>(& b) };
				points.push_back( AL_PNT{ pBaseKnot } );
			}
		} 
	);

	BaseKnot const * pStart { nullptr };
	BaseKnot const * pEnd   { nullptr };

	// find 2 baseknots (START/END) with maximum distance

	MicroMeter maxDist { 0.0_MicroMeter };
	for ( auto & it1 : points )
		for ( auto & it2 : points )
		{
			MicroMeter dist { Distance( it1.pBaseKnot->GetPosition(), it2.pBaseKnot->GetPosition() ) };
			if ( dist > maxDist )
			{
				maxDist = dist;
				pStart = it1.pBaseKnot;
				pEnd   = it2.pBaseKnot;
			}
		}

	// for every other baseknot compute position on line START to END

	MicroMeterLine line      { pStart->GetPosition(), pEnd->GetPosition() };
	MicroMeterLine orthoLine { line.OrthoLine() };
	for ( auto & it : points )
		it.umDist = PointToLine( orthoLine, it.pBaseKnot->GetPosition() );

	// sort baseknots according to position

	sort( points.begin(), points.end(), [](auto & p1, auto & p2){ return p1.umDist < p2.umDist; } );

	// compute tightly packed positions

	float           fNrOfPoints       { Cast2Float(points.size()) };
	MicroMeter      umOriginLength    { line.Length() };
	MicroMeter      umTargetLength    { NEURON_RADIUS * 2.0f * fNrOfPoints };
	MicroMeterPoint umPntPackedSingle { line.GetVector() * (umTargetLength / umOriginLength) / fNrOfPoints };
	MicroMeterPoint umPntTargetCenter { (pEnd->GetPosition() + pStart->GetPosition()) * 0.5f };
	MicroMeterPoint umPntTargetStart  { umPntTargetCenter - umPntPackedSingle * fNrOfPoints * 0.5f };
	MicroMeterPoint umPntTargetEnd    { umPntTargetCenter + umPntPackedSingle * fNrOfPoints * 0.5f };

	// fill animation vectors

	MicroMeterPointVector umPntVectorActual;
	MicroMeterPointVector umPntVectorTarget;

	m_shapesAnimated.Clear();

	for ( int i = 0; i < points.size(); ++i )
	{
		umPntVectorTarget.Add( umPntTargetStart + umPntPackedSingle * static_cast<float>(i), Degrees::ZERO_VAL() );
		umPntVectorActual.Add( points[i].pBaseKnot->GetPosition(), Degrees::ZERO_VAL() );
		m_shapesAnimated.Add( points[i].pBaseKnot );
	}

	m_upConnectorAnimation->Start( umPntVectorActual, umPntVectorTarget );
	return true;
}

void AlignAnimation::AnimationStep( )
{
	m_pNNetCommands->SetBaseKnots
	(
		m_upConnectorAnimation->GetActual(), 
		m_shapesAnimated
	);
}
