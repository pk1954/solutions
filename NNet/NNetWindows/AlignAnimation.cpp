// AlignAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ShapeIdList.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "CalcOrthoVector.h"
#include "win32_sound.h"
#include "AlignAnimation.h"

void AlignAnimation::Initialize
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

bool const AlignAnimation::prepareData()
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

void AlignAnimation::AlignSelection( AnimationScript const & script )
{
	m_pScript = & script;
	m_iScriptStep = 0;

	if ( prepareData() )
		scriptStep();
	else 
		m_pSound->Play( TEXT("NOT_POSSIBLE_SOUND") );
}

void AlignAnimation::scriptStep()
{
	MicroMeterPointVector const umPntVectorStart (m_shapesAnimated);
	MicroMeterPointVector       umPntVectorTarget(m_shapesAnimated);

	DWORD const dwOptions { m_pScript->at(m_iScriptStep) };
	if (dwOptions & ALIGN_DIRECTION)
	{
		umPntVectorTarget.SetDir(Vector2Radian(CalcOrthoVector(m_line, m_shapesAnimated)));
	}
	else if ( dwOptions & ALIGN_SHAPES )
	{
		umPntVectorTarget.Align(m_line);
	}
	else if ( dwOptions & PACK_SHAPES )
	{		
		umPntVectorTarget.Pack(NEURON_RADIUS * 1.8f);
	}

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

bool const AlignAnimation::NextScriptStep()
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
