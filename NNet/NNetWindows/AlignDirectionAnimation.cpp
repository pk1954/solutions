// AlignDirectionAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ShapeIdList.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "CalcOrthoVector.h"
#include "AlignDirectionAnimation.h"

void AlignDirectionAnimation::Initialize
( 
	HWND               const   hwnd,
	NNetModelWriterInterface & nmwi,
	NNetModelCommands        & commands,
	Sound                    * pSound
)
{
	m_pNMWI           = & nmwi;
	m_pModelCommands  = & commands;
	m_upConnAnimation = make_unique<ConnAnimation>(IDX_CONNECTOR_ANIMATION, hwnd);
	m_pSound          = pSound;
}

bool const AlignDirectionAnimation::prepareData()
{
	m_line.Set2Null();

	ShapeType const shapeType { m_pNMWI->GetUPShapes().DetermineShapeType() };
	if ( shapeType.IsUndefinedType() )
		return false;

	m_shapesAnimated = m_pNMWI->GetUPShapes().GetAllSelected<ConnNeuron>(shapeType);
	m_line = m_shapesAnimated.CalcMaxDistLine();
	if ( m_line.IsZero() )
		return false;

	m_shapesAnimated.SortAccToDistFromLine( m_line.OrthoLine() );
	return true;
}

void AlignDirectionAnimation::StartAnimation()
{
	if ( ! prepareData() )
	{
		m_pSound->Play( TEXT("NOT_POSSIBLE_SOUND") );
		return;
	}
	doAnimation();
}

void AlignDirectionAnimation::doAnimation()
{
	MicroMeterPointVector const umPntVectorStart (m_shapesAnimated);
	MicroMeterPointVector       umPntVectorTarget(m_shapesAnimated);

	umPntVectorTarget.SetDir(Vector2Radian(CalcOrthoVector(m_line, m_shapesAnimated)));

	m_upConnAnimation->SetNrOfSteps( calcNrOfSteps(umPntVectorStart, umPntVectorTarget) );
	m_upConnAnimation->Start(umPntVectorStart, umPntVectorTarget);
}

unsigned int const AlignDirectionAnimation::calcNrOfSteps
(
	MicroMeterPointVector const & umPntVectorStart,
	MicroMeterPointVector const & umPntVectorTarget
) const
{
	MicroMeterPointVector const umPntVectorDiff { umPntVectorTarget - umPntVectorStart };
	Radian                const radDiffMax      { umPntVectorDiff.FindMaxRadian() };
	Radian                const radPerStep      { Degrees2Radian(6.0_Degrees) };
	float                 const fSteps          { radDiffMax / radPerStep };
	unsigned int          const uiSteps         { Cast2UnsignedInt(fSteps) + 1 };
	return uiSteps;
}

void AlignDirectionAnimation::AnimationStep()
{
	unique_ptr<ShapeIdList> upShapeIds { make_unique<ShapeIdList>(m_shapesAnimated) };

	m_pModelCommands->SetConnectionNeurons
	(
		m_upConnAnimation->GetActual(), 
		move(upShapeIds)
	);
}
