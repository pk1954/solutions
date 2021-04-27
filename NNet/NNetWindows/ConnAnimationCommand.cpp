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
    RootWindow                   const * pWin,
    int                          const   iStep,
    bool                         const   bBackwards
)
  : m_upShapesAnimated(move(upShapesAnimated)),
    m_pWin(pWin),
    m_iStep(iStep),
    m_bBackwards(bBackwards)
{}

void ConnAnimationCommand::initialize( NNetModelWriterInterface& nmwi )
{
    m_upConnAnimation = make_unique<Animation<MicroMeterPointVector>>
    (
        [&](bool const bTargetReached) 
        { 
            m_pWin->PostMessage(WM_USER, IDX_ANIMATION_STEP, reinterpret_cast<LPARAM>(this));
            if (TargetReached())
            {
                if (Forwards())
                {
                    if (m_iStep)
                        m_pWin->PostMessage(WM_USER, IDX_MAKE_CON_STEP, m_iStep);
                }
                else
                {
                    if (m_bBackwards)
                        m_pWin->PostCommand(IDM_UNDO);
                }
            }
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
    m_bForwards = true;
    m_upConnAnimation->Start(m_umPntVectorStart, m_umPntVectorTarget);
}

void ConnAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_bForwards = false;
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
    m_line = m_upShapesAnimated->CalcMaxDistLine();
    if ( m_line.IsZero() )
        return false;

    m_upShapesAnimated->SortAccToDistFromLine( m_line.OrthoLine() );
    return true;
}
