// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include "win32_Commands.h"
#include "NNetModelWriterInterface.h"
#include "IoNeuron.h"
#include "IoNeuronList.h"
#include "MultiNobsAnimation.h"
#include "MakeConnAnimation.h"
#include "ConnAnimationCommand.h"

using std::make_unique;

ConnAnimationCommand::ConnAnimationCommand
(  
    MainWindow  & win,
    WinCommands & cmds
)
  : AnimationSequence(win)
{
    NNetModelWriterInterface & nmwi      { cmds.GetNMWI() };
    UPNobList                & modelNobs { nmwi.GetUPNobs() };
    NobType              const nobType   { determineNobType(modelNobs) };
    if ( nobType.IsUndefinedType() )
        return;
    
    unique_ptr<IoNeuronList> upNobsAnimated { make_unique<IoNeuronList>() };
    modelNobs.Apply2All<IoNeuron>
    ( 
        [&](IoNeuron & s)	
        { 
            if (s.IsSelected() && s.HasType(nobType)) 
                upNobsAnimated->Add(&s); 
        } 
    );
    MicroMeterLine line{ upNobsAnimated->CalcMaxDistLine() };
    if (line.IsZero())
        return;

    upNobsAnimated->SortAccToDistFromLine( line.OrthoLine() );
    
    MicroMeterPntVector umPntVector(*upNobsAnimated.get());  // before animation

    umPntVector.Align(line);
    AddPhase(make_unique<MultiNobsAnimation>(win, *upNobsAnimated.get(), umPntVector));  // after position alignment

    umPntVector.SetDir(Vector2Radian(upNobsAnimated->CalcOrthoVector(line)));
    AddPhase(make_unique<MultiNobsAnimation>(win, *upNobsAnimated.get(), umPntVector));  // after direction alignment

    umPntVector.Pack(NEURON_RADIUS * 2.0f);
    AddPhase(make_unique<MultiNobsAnimation>(win, *upNobsAnimated.get(), umPntVector));  // after packing

    AddPhase(make_unique<MakeConnAnimation>(win, nmwi, move(upNobsAnimated)));

    m_bAllOk = true;
}

NobType const ConnAnimationCommand::determineNobType(UPNobList const & nobs) const
{
    unsigned int uiNrOfIoConnectors { nobs.CountInSelection( NobType::Value::connector ) };

    if ( uiNrOfIoConnectors > 0 )
        return NobType::Value::undefined;

    unsigned int uiNrOfInputNeurons  { nobs.CountInSelection( NobType::Value::inputNeuron  ) };
    unsigned int uiNrOfOutputNeurons { nobs.CountInSelection( NobType::Value::outputNeuron ) };

    if ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
        return NobType::Value::undefined;

    return (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
        ? NobType::Value::inputNeuron 
        : NobType::Value::outputNeuron;
}
