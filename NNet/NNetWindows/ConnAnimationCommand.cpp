// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include <algorithm>
#include "win32_Commands.h"
#include "NNetModelWriterInterface.h"
#include "IoNeuron.h"
#include "IoNeuronsAnimation.h"
#include "MakeConnAnimation.h"
#include "ConnAnimationCommand.h"

using std::make_unique;
using std::sort;

ConnAnimationCommand::ConnAnimationCommand
( 
    MainWindow  & win,
    WinCommands & cmds
)
  : AnimationSequence(win)
{
    UPNobList   & modelNobs { cmds.GetNMWI().GetUPNobs() };
    NobType const nobType   { determineNobType(modelNobs) };
    if (nobType.IsUndefinedType())
        return;
    
    modelNobs.Apply2All<IoNeuron>
    (
        [&](IoNeuron & s)	
        { 
            if (s.IsSelected() && (s.GetNobType() == nobType)) 
                m_nobsAnimated.push_back(&s); 
        } 
    );
    MicroMeterLine line{ CalcMaxDistLine<IoNeuron>(m_nobsAnimated) };
    if (line.IsZero())
        return;

    MicroMeterLine orthoLine { line.OrthoLine() };

    sort
    (
        m_nobsAnimated.begin(), m_nobsAnimated.end(),
        [&](auto & p1, auto & p2) 
        { 
            return PointToLine(orthoLine, p1->GetPos()) < PointToLine(orthoLine, p2->GetPos()); 
        } 
    );

    MicroMeterPntVector umPntVector(m_nobsAnimated);  // before animation

    umPntVector.Align(line);
    AddPhase(make_unique<IoNeuronsAnimation>(win, m_nobsAnimated, umPntVector));  // after position alignment

    umPntVector.SetDir(Vector2Radian(CalcOrthoVector<IoNeuron>(m_nobsAnimated, line)));
    AddPhase(make_unique<IoNeuronsAnimation>(win, m_nobsAnimated, umPntVector));  // after direction alignment

    umPntVector.Pack(NEURON_RADIUS * 2.0f);
    AddPhase(make_unique<IoNeuronsAnimation>(win, m_nobsAnimated, umPntVector));  // after packing

    AddPhase(make_unique<MakeConnAnimation>(win, move(m_nobsAnimated)));

    m_bAllOk = true;
}

NobType const ConnAnimationCommand::determineNobType(UPNobList const & nobs) const
{
    if (nobs.CountInSelection(NobType::Value::inputConnector) > 0)
        return NobType::Value::undefined;

    if (nobs.CountInSelection(NobType::Value::outputConnector) > 0)
        return NobType::Value::undefined;

    unsigned int uiNrOfInputNeurons  { nobs.CountInSelection(NobType::Value::inputNeuron ) };
    unsigned int uiNrOfOutputNeurons { nobs.CountInSelection(NobType::Value::outputNeuron) };

    if ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
        return NobType::Value::undefined;

    return (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
        ? NobType::Value::inputNeuron 
        : NobType::Value::outputNeuron;
}
