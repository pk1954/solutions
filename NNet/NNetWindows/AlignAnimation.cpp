// AlignAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ShapeIdList.h"
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

bool const AlignAnimation::prepareData()
{
	m_line.Set2Null();

	ShapeType const shapeType { m_pNMWI->GetUPShapes().DetermineShapeType() };
	if ( shapeType.IsUndefinedType() )
		return false;

	m_shapesAnimated = m_pNMWI->GetUPShapes().GetAllSelected<ConnNeuron>();
	m_line = m_shapesAnimated.CalcMaxDistLine();
	if ( m_line.IsNull() )
		return false;

	m_shapesAnimated.SortAccToDistFromLine( m_line.OrthoLine() );
	return true;
}

bool const AlignAnimation::AlignSelection( AnimationScript const & script )
{
	m_pScript = & script;
	m_iScriptStep = 0;

	if ( ! prepareData() )
		return false;

	scriptStep();
	return true;
}

void AlignAnimation::scriptStep()
{
	DWORD const dwOptions { m_pScript->at(m_iScriptStep) };
	bool bAlignDirection = dwOptions & ALIGN_DIRECTION;
	bool bAlignShapes    = dwOptions & (ALIGN_SHAPES|PACK_SHAPES);
	bool bPackShapes     = dwOptions & PACK_SHAPES;

	float      const fGapCount          { Cast2Float(m_shapesAnimated.Size() - 1) };
	MicroMeter const umUnpackedDistance { m_line.Length() / fGapCount };
	MicroMeter const umShapeDistTarget  { bPackShapes ? NEURON_RADIUS * 1.8f : umUnpackedDistance };
	MicroMeter const umLineLengthTarget { umShapeDistTarget * fGapCount };

	// compute target positions (packed if PACK_SHAPES is active)

	MicroMeterPoint umPntSingleVector { m_line.GetVector().ScaledTo(umShapeDistTarget) };
	MicroMeterPoint umPntLineTarget   { m_line.GetVector().ScaledTo(umLineLengthTarget) };
	MicroMeterPoint umPntTargetStart  { m_line.GetCenter() - umPntLineTarget * 0.5f };

	// fill animation vectors

	MicroMeterPoint       orthoVector { CalcOrthoVector(m_line, m_shapesAnimated) };
	MicroMeterPosDir      posDirTarget { umPntTargetStart, Vector2Radian(orthoVector) };
	MicroMeterPointVector umPntVectorTarget {};
	m_shapesAnimated.Apply2All
	(	
		[&](ConnNeuron const & c)	
		{ 
			MicroMeterPosDir posDir 
			{
				bAlignShapes    ? posDirTarget.GetPos() : c.GetPos(), 
				bAlignDirection ? posDirTarget.GetDir() : Vector2Radian(c.GetDirVector()) 
			};
			umPntVectorTarget.Add( posDir );
			posDirTarget += umPntSingleVector;
		}	
	);
	MicroMeterPointVector umPntVectorStart( m_shapesAnimated );
	m_upConnAnimation->SetNrOfSteps( calcNrOfSteps(umPntVectorStart, umPntVectorTarget) );
	m_upConnAnimation->Start(umPntVectorStart, umPntVectorTarget);
}

unsigned int const AlignAnimation::calcNrOfSteps
(
	MicroMeterPointVector const & umPntVectorStart,
	MicroMeterPointVector const & umPntVectorTarget
) const
{
	MicroMeterPointVector const umPntVectorDiff { umPntVectorTarget - umPntVectorStart };
	Radian                const radDiffMax      { umPntVectorDiff.FindMaxRadian() };
	Radian                const radPerStep      { Degrees2Radian(6.0_Degrees) };
	float                 const fStepsFromRot   { radDiffMax / radPerStep };

	MicroMeter            const umDiffMax       { umPntVectorDiff.FindMaxPos() };
	MicroMeter            const umPerStep       { NEURON_RADIUS / 5.0f };
	float                 const fStepsFromMove  { umDiffMax / umPerStep };

	float                 const fSteps          { max(fStepsFromRot, fStepsFromMove) };
	unsigned int          const uiSteps         { Cast2UnsignedInt(fSteps) + 1 };
	return uiSteps;
}

void AlignAnimation::AnimationStep()
{
	unique_ptr<ShapeIdList> upShapeIds { make_unique<ShapeIdList>(m_shapesAnimated) };

	m_pModelCommands->SetConnectionNeurons
	(
		m_upConnAnimation->GetActual(), 
		move(upShapeIds)
	);
}

bool const AlignAnimation::NextStep()
{
	return ++m_iScriptStep < m_pScript->size();
}

wchar_t const * const AlignAnimation::DoNextStep()
{
	if (m_pScript->at(m_iScriptStep) & CREATE_CONNECTOR)
	{
		m_pModelCommands->CreateConnector( m_shapesAnimated );
		return L"SNAP_IN_SOUND";
	}
	else
	{
		scriptStep();
		return nullptr;
	}
}
