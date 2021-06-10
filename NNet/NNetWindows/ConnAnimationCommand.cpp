// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include "CalcOrthoVector.h"
#include "win32_Commands.h"
#include "MultiNobsAnimation.h"
#include "MakeConnAnimation.h"
#include "ConnAnimationCommand.h"

using std::make_unique;

class NNetModelWriterInterface;

ConnAnimationCommand::ConnAnimationCommand
(  
    MainWindow  & win,
    WinCommands & cmds
)
  : AnimationSequence(win)
{
    NNetModelWriterInterface & nmwi      { cmds.GetNMWI() };
    UPNobList                & modelNobs { nmwi.GetUPNobs() };
    NobType const nobType { determineNobType(modelNobs) };
    if ( nobType.IsUndefinedType() )
        return;
    
    NobPtrList<IoNeuron> nobsAnimated { NobPtrList<IoNeuron>(modelNobs.GetAllSelected<IoNeuron>(nobType)) };
    MicroMeterLine       line         { nobsAnimated.CalcMaxDistLine() };
    if (line.IsZero())
        return;

    nobsAnimated.SortAccToDistFromLine( line.OrthoLine() );
    
    MicroMeterPntVector umPntVector(nobsAnimated);  // before animation

    umPntVector.Align(line);
    AddPhase(make_unique<MultiNobsAnimation>(win, nobsAnimated, umPntVector));  // after position alignment

    umPntVector.SetDir(Vector2Radian(CalcOrthoVector(line, nobsAnimated)));
    AddPhase(make_unique<MultiNobsAnimation>(win, nobsAnimated, umPntVector));  // after direction alignment

    umPntVector.Pack(NEURON_RADIUS * 2.0f);
    AddPhase(make_unique<MultiNobsAnimation>(win, nobsAnimated, umPntVector));  // after packing

    AddPhase(make_unique<MakeConnAnimation >(win, nmwi));

    m_bAllOk = true;
}

NobType const ConnAnimationCommand::determineNobType(UPNobList const & nobs) const
{
    unsigned int uiNrOfConnectors { nobs.CountInSelection( NobType::Value::connector ) };

    if ( uiNrOfConnectors > 0 )
        return NobType::Value::undefined;

    unsigned int uiNrOfInputNeurons  { nobs.CountInSelection( NobType::Value::inputNeuron  ) };
    unsigned int uiNrOfOutputNeurons { nobs.CountInSelection( NobType::Value::outputNeuron ) };

    if ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
        return NobType::Value::undefined;

    return (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
        ? NobType::Value::inputNeuron 
        : NobType::Value::outputNeuron;
}
