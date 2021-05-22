// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "CalcOrthoVector.h"
#include "win32_rootWindow.h"
#include "win32_Commands.h"
#include "CommandStack.h"
#include "NobIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "ConnAnimationCommand.h"

using std::make_unique;

ConnAnimationCommand::ConnAnimationCommand
( 
    RootWindow  & win,
    WinCommands & cmds
)
  : m_win(win),
    m_NMWI(cmds.GetNMWI()),
    m_callable(win.GetWindowHandle())
{
    m_pModelNobs = &m_NMWI.GetUPNobs();
    NobType const nobType { determineNobType() };
    if ( nobType.IsUndefinedType() )
        return;
    
    m_nobsAnimated = NobPtrList<IoNeuron>(m_pModelNobs->GetAllSelected<IoNeuron>(nobType));

    MicroMeterLine line { MicroMeterLine::NULL_VAL() };
    line = m_nobsAnimated.CalcMaxDistLine();
    if (line.IsZero())
        return;

    m_nobsAnimated.SortAccToDistFromLine( line.OrthoLine() );
    m_upConnector  = make_unique<Connector>( m_nobsAnimated );
    
    m_umPntVectorOriginal = MicroMeterPntVector( m_nobsAnimated );

    m_umPntVectorTarget1  = m_umPntVectorOriginal;
    m_umPntVectorTarget1.Align(line);

    m_umPntVectorTarget2  = m_umPntVectorTarget1;
    m_umPntVectorTarget2.SetDir(Vector2Radian(CalcOrthoVector(line, m_nobsAnimated)));

    m_umPntVectorTarget3  = m_umPntVectorTarget2;
    m_umPntVectorTarget3.Pack(NEURON_RADIUS * 2.0f);

    m_bAllOk = true;
}

NobType const ConnAnimationCommand::determineNobType() const
{
    unsigned int uiNrOfConnectors { m_pModelNobs->CountInSelection( NobType::Value::connector ) };

    if ( uiNrOfConnectors > 0 )
        return NobType::Value::undefined;

    unsigned int uiNrOfInputNeurons  { m_pModelNobs->CountInSelection( NobType::Value::inputNeuron  ) };
    unsigned int uiNrOfOutputNeurons { m_pModelNobs->CountInSelection( NobType::Value::outputNeuron ) };

    if ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
        return NobType::Value::undefined;

    return (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
        ? NobType::Value::inputNeuron 
        : NobType::Value::outputNeuron;
}

void ConnAnimationCommand::updateUI()  // runs in animation thread
{
    m_NMWI.SetIoNeurons
    (
        m_connAnimation.GetActual(), 
        m_nobsAnimated
    );
    m_win.Notify(false);
}

void ConnAnimationCommand::nextAnimationPhase() // runs in UI thread
{
    MicroMeterPntVector umPntVectorStart { MicroMeterPntVector( m_nobsAnimated ) };
    MicroMeterPntVector umPntVectorTarget;

    if (m_mode == Mode::mode_do)
    {
        switch (m_iPhase++)
        {
        case 1:  BlockUI();
                 umPntVectorTarget = m_umPntVectorTarget1; break;
        case 2:	 umPntVectorTarget = m_umPntVectorTarget2; break;
        case 3:  umPntVectorTarget = m_umPntVectorTarget3; break;
        case 4:	 m_upConnector->SetParentPointers();
                 m_pModelNobs->Push(move(m_upConnector));
                 UnblockUI();
                 return; 
        default: return;        // do not start animation
        }
    }
    else
    {
        switch (m_iPhase--)
        {
        case 4:	 BlockUI();
                 m_upConnector = m_pModelNobs->Pop<Connector>();
                 m_upConnector->ClearParentPointers();
                 [[fallthrough]]; 
        case 3:	 umPntVectorTarget = m_umPntVectorTarget2;  break;
        case 2:	 umPntVectorTarget = m_umPntVectorTarget1;  break;
        case 1:	 umPntVectorTarget = m_umPntVectorOriginal; break;
        case 0:  UnblockUI();
                 return; 
        default: return;                // do not start animation
        }
    }
    m_connAnimation.SetNrOfSteps( CalcNrOfSteps(umPntVectorStart, umPntVectorTarget) );
    m_connAnimation.Start(umPntVectorStart, umPntVectorTarget);
}

void ConnAnimationCommand::Do( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_do;
    m_iPhase = 1;
    nextAnimationPhase();
}

void ConnAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_undo;
    m_iPhase = 4;
    nextAnimationPhase();
}
