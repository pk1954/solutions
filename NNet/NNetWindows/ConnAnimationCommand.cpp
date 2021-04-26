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
    unique_ptr<ShapePtrList<ConnNeuron>> upShapesAnimated,
    RootWindow                   * const pWindow,
    function<void(bool const)>     const finFunc
)
  : m_upShapesAnimated(move(upShapesAnimated)),
    m_pWindow(pWindow),
    m_finFunc(finFunc)
{
    if ( m_upShapesAnimated.get( ) == nullptr )
    {
        int x = 42;
    }
}

void ConnAnimationCommand::initialize( NNetModelWriterInterface& nmwi )
{
    m_upConnAnimation = make_unique<Animation<MicroMeterPointVector>>
    (
        [&](bool const bTargetReached, bool const bForwards)
        { 
            nmwi.SetConnNeurons(m_upConnAnimation->GetActual(), * m_upShapesAnimated.get());
            m_pWindow->Notify(false);
            if (bTargetReached)
                (m_finFunc)(bForwards);
        }
    );
    prepareData(nmwi);
    m_umPntVectorStart  = MicroMeterPointVector(*m_upShapesAnimated.get());
    m_umPntVectorTarget = MicroMeterPointVector(*m_upShapesAnimated.get());
    DefineTarget();
    m_upConnAnimation->SetNrOfSteps( calcNrOfSteps(m_umPntVectorStart, m_umPntVectorTarget) );
    m_bInitialized = true;
}

void ConnAnimationCommand::Do( NNetModelWriterInterface& nmwi )
{
    if ( ! m_bInitialized )
        initialize(nmwi);
    m_upConnAnimation->Start(m_umPntVectorStart, m_umPntVectorTarget, true);
}

void ConnAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_upConnAnimation->Start(m_umPntVectorTarget, m_umPntVectorStart, false);
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
    m_line = m_upShapesAnimated->CalcMaxDistLine();
    if ( m_line.IsZero() )
        return false;

    m_upShapesAnimated->SortAccToDistFromLine( m_line.OrthoLine() );
    return true;
}
