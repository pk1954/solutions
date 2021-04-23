// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "win32_rootWindow.h"
#include "ShapeIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "ConnAnimationCommand.h"

using std::make_unique;

ConnAnimationCommand::ConnAnimationCommand
( 
    RootWindow        * const pWindow,
    NNetModelCommands * const pCmds
)
  : m_pModelCommands(pCmds),
    m_pWindow(pWindow)
{}

void ConnAnimationCommand::initialize( NNetModelWriterInterface& nmwi )
{
    m_upConnAnimation = make_unique<ConnAnimation>
    (
        [&](bool const bTargetReached)
        { 
            nmwi.SetConnNeurons(m_upConnAnimation->GetActual(), m_shapesAnimated);
            m_pWindow->Notify(false);
        }
    );
    prepareData(nmwi);
    m_umPntVectorStart  = MicroMeterPointVector(m_shapesAnimated);
    m_umPntVectorTarget = MicroMeterPointVector(m_shapesAnimated);
    DefineTarget();
    m_upConnAnimation->SetNrOfSteps( calcNrOfSteps(m_umPntVectorStart, m_umPntVectorTarget) );
    m_bInitialized = true;
}

void ConnAnimationCommand::Do( NNetModelWriterInterface& nmwi )
{
    if ( ! m_bInitialized )
        initialize(nmwi);
    m_upConnAnimation->Start(m_umPntVectorStart, m_umPntVectorTarget);
}

void ConnAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_upConnAnimation->Start(m_umPntVectorTarget, m_umPntVectorStart);
}

unsigned int const ConnAnimationCommand::calcNrOfSteps
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

bool const ConnAnimationCommand::prepareData(NNetModelWriterInterface& nmwi)
{
    m_line.Set2Null();

    ShapeType const shapeType { nmwi.GetUPShapes().DetermineShapeType() };
    if ( shapeType.IsUndefinedType() )
        return false;

    m_shapesAnimated = nmwi.GetUPShapes().GetAllSelected<ConnNeuron>(shapeType);
    m_line = m_shapesAnimated.CalcMaxDistLine();
    if ( m_line.IsZero() )
        return false;

    m_shapesAnimated.SortAccToDistFromLine( m_line.OrthoLine() );
    return true;
}
