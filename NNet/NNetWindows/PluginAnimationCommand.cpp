// PluginAnimationCommand.cpp
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
#include "PluginAnimationCommand.h"

using std::make_unique;

PluginAnimationCommand::PluginAnimationCommand
( 
    Connector   & connAnimated,
    Connector   & connTarget,
    MainWindow  & win,
    WinCommands & cmds
)
  : m_connTarget(connTarget),
    m_connAnimated(connAnimated),
    m_win(win),
    m_NMWI(cmds.GetNMWI()),
    m_callable(win.GetWindowHandle())
{
    m_pModelNobs = &m_NMWI.GetUPNobs();

    Radian          const radianTarget { m_connTarget.GetDir() };
    MicroMeterPoint const umDirVector  { Radian2Vector(radianTarget).ScaledTo(NEURON_RADIUS) };
    
    m_umPosDirTarget[0] = m_connAnimated.GetPosDir();

    array <float, 2> fOffsets { 5.0f, 2.3f };

    for (int i = 1; i<= 2; ++i )
    {
        float fOff { fOffsets[i-1] };
        if (m_connTarget.IsOutputConnector() )
            fOff = -fOff;
        MicroMeterPoint const umPosOffset { umDirVector * fOff };
        MicroMeterPoint const umPosTarget { m_connTarget.GetPos() - umPosOffset };
        m_umPosDirTarget[i] = MicroMeterPosDir( umPosTarget, radianTarget );
    }

    m_upClosedConnector = make_unique<ClosedConnector>(m_connTarget, m_connAnimated);
}

void PluginAnimationCommand::updateUI()  // runs in animation thread
{
    m_connAnimated.SetPosDir(m_pluginAnimation.GetActual());
    m_win.Notify(false);
}

void PluginAnimationCommand::nextAnimationPhase() // runs in UI thread
{
    MicroMeterPosDir umPosDirStart { m_connAnimated.GetPosDir() };
    MicroMeterPosDir umPosDirTarget;

    if (m_mode == Mode::mode_do)
    {
        switch (m_iPhase++)
        {
        case 1:  blockUI();
                 umPosDirTarget = m_umPosDirTarget[1]; break;
        case 2:	 umPosDirTarget = m_umPosDirTarget[2]; break;
        case 3:  m_upClosedConnector->SetParentPointers();
                 m_pModelNobs->Push(move(m_upClosedConnector));
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
                 m_upClosedConnector = m_pModelNobs->Pop<ClosedConnector>();
                 m_upClosedConnector->ClearParentPointers();
                 [[fallthrough]]; 
        case 2:  umPosDirTarget = m_umPosDirTarget[1]; break;
        case 1:	 umPosDirTarget = m_umPosDirTarget[0]; break;
        case 0:  unblockUI();
                 return; 
        default: return;                // do not start animation
        }
    }
    m_pluginAnimation.SetNrOfSteps( calcNrOfSteps(umPosDirStart, umPosDirTarget) );
    m_pluginAnimation.Start(umPosDirStart, umPosDirTarget);
    m_win.Notify(false);
}

void PluginAnimationCommand::Do( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_do;
    m_iPhase = 1;
    nextAnimationPhase();
}

void PluginAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_undo;
    m_iPhase = 3;
    nextAnimationPhase();
}

unsigned int const PluginAnimationCommand::calcNrOfSteps
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
