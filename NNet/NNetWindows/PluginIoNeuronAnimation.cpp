// PluginIoNeuronAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "MoreTypes.h"
#include "CalcOrthoVector.h"
#include "win32_Commands.h"
#include "CommandStack.h"
#include "NobIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "PluginIoNeuronAnimation.h"

using std::make_unique;

PluginIoNeuronAnimation::PluginIoNeuronAnimation
( 
    IoNeuron    & nobAnimated,
    IoNeuron    & nobTarget,
    MainWindow  & win,
    WinCommands & cmds
)
  : m_nobTarget(nobTarget),
    m_nobAnimated(nobAnimated),
    m_win(win),
    m_NMWI(cmds.GetNMWI()),
    m_callable(win.GetWindowHandle())
{
    assert( m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob() );
    assert( m_nobAnimated.GetIoMode() != NobIoMode::internal );
    assert( m_nobTarget  .GetIoMode() != NobIoMode::internal );
    assert( m_nobTarget  .GetIoMode() !== m_nobAnimated.GetIoMode() );

    Radian        const radianTarget { m_nobTarget.GetDir() };
    MicroMeterPnt const umDirVector  { Radian2Vector(radianTarget).ScaledTo(NEURON_RADIUS) };

    m_umPosDirTarget[0] = m_nobAnimated.GetPosDir();

    array <float, 2> fOffsets { 5.0f, 1.4f };

    for (size_t i = 1; i<= 2; ++i )
    {
        float fOff { fOffsets[i-1] };
        if (m_nobTarget.GetIoMode() == NobIoMode::input )
            fOff = -fOff;
        MicroMeterPnt const umPosOffset { umDirVector * fOff };
        MicroMeterPnt const umPosTarget { m_nobTarget.GetPos() + umPosOffset };
        m_umPosDirTarget[i] = MicroMeterPosDir( umPosTarget, radianTarget );
    }

    IoNeuron * pNobIn;
    IoNeuron * pNobOut;

    if ( m_nobAnimated.IsInternalNob() )
    {
        pNobIn  = &m_nobAnimated;
        pNobOut = &m_nobTarget; 
    }
    else
    {
        pNobIn  = &m_nobTarget;
        pNobOut = &m_nobAnimated; 
    }

    m_upClosedNeuron = move(make_unique<Neuron>(m_nobTarget.GetPos(), * pNobIn, * pNobOut));
}

void PluginIoNeuronAnimation::updateUI()  // runs in animation thread
{
    m_nobAnimated.SetPosDir(m_pluginAnimation.GetActual());
    m_win.Notify(false);
}

void PluginIoNeuronAnimation::nextAnimationPhase() // runs in UI thread
{
    MicroMeterPosDir umPosDirStart { m_nobAnimated.GetPosDir() };
    MicroMeterPosDir umPosDirTarget;

    if (m_mode == Mode::mode_do)
    {
        switch (m_iPhase++)
        {
        case 1:  blockUI();
                 umPosDirTarget = m_umPosDirTarget[1]; break;
        case 2:	 umPosDirTarget = m_umPosDirTarget[2]; break;
        case 3:  m_upClosedNeuron->SetParentPointers();
                {
                    NobId id = m_NMWI.GetUPNobs().Push(move(m_upClosedNeuron));
                    m_upNobAnimated = m_NMWI.RemoveFromModel<IoNeuron>(m_nobAnimated);
                    m_upNobTarget   = m_NMWI.RemoveFromModel<IoNeuron>(m_nobTarget);
                    m_NMWI.GetNobPtr<Neuron *>(id)->Reconnect();
                }
                unblockUI();
                return; 
        default: return;        // do not start animation
        }
    }
    else
    {
        switch (m_iPhase--)
        {
        case 3:  blockUI();
            m_upClosedNeuron = m_NMWI.GetUPNobs().Pop<Neuron>();
            m_upClosedNeuron->ClearParentPointers();
            [[fallthrough]]; 
        case 2:  umPosDirTarget = m_umPosDirTarget[1]; break;
        case 1:	 umPosDirTarget = m_umPosDirTarget[0]; break;
        case 0:  m_upNobAnimated->Reconnect();
                 m_upNobTarget  ->Reconnect();
                 m_upNobAnimated = m_NMWI.ReplaceInModel<IoNeuron,IoNeuron>(move(m_upNobAnimated));
                 m_upNobTarget   = m_NMWI.ReplaceInModel<IoNeuron,IoNeuron>(move(m_upNobTarget));
                 unblockUI();
                 return; 
        default: return;                // do not start animation
        }
    }
    m_pluginAnimation.SetNrOfSteps( calcNrOfSteps(umPosDirStart, umPosDirTarget) );
    m_pluginAnimation.Start(umPosDirStart, umPosDirTarget);
    m_win.Notify(false);
}

void PluginIoNeuronAnimation::Do( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_do;
    m_iPhase = 1;
    nextAnimationPhase();
}

void PluginIoNeuronAnimation::Undo( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_undo;
    m_iPhase = 3;
    nextAnimationPhase();
}

unsigned int const PluginIoNeuronAnimation::calcNrOfSteps
(
    MicroMeterPosDir const & umPosDirStart,
    MicroMeterPosDir const & umPosDirTarget
) const
{
    MicroMeterPosDir const umPosDirDiff   { umPosDirTarget - umPosDirStart };

    Radian           const radPerStep     { Degrees2Radian(6.0_Degrees) };
    float            const fStepsFromRot  { Normalize(umPosDirDiff.GetDir()) / radPerStep };

    MicroMeter       const umPerStep      { NEURON_RADIUS / 5.0f };
    float            const fStepsFromMove { Hypot(umPosDirDiff.GetPos()) / umPerStep };

    float            const fSteps         { max(fStepsFromRot, fStepsFromMove) };
    unsigned int     const uiSteps        { Cast2UnsignedInt(fSteps) + 1 };
    return uiSteps;
}
