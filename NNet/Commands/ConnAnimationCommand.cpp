// ConnAnimationCommand.cpp
//
// Command

#include "stdafx.h"
#include <algorithm>
#include "NNetModelWriterInterface.h"
#include "IoLine.h"
#include "IoLinesAnimation.h"
#include "MakeConnAnimation.h"
#include "ConnAnimationCommand.h"

using std::make_unique;
using std::ranges::sort;

ConnAnimationCommand::ConnAnimationCommand()
{
    UPNobList   & modelNobs { m_pNMWI->GetUPNobs() };
    NobType const nobType   { determineNobType(modelNobs) };
    if (nobType.IsUndefinedType())
        return;
    
    modelNobs.Apply2All<IoLine>
    (
        [this, nobType](IoLine & s)	
        { 
            if (s.IsSelected() && (s.GetNobType() == nobType)) 
                m_nobsAnimated.push_back(&s); 
        } 
    );

    MicroMeterLine line { CalcMaxDistLine<IoLine>(m_nobsAnimated) };
    if (line.IsZero())
        return;

    sortNobsAnimated(line);

    MicroMeterPntVector umPntVector(m_nobsAnimated);  // before animation

    umPntVector.Align(line);
    AddPhase(make_unique<IoLinesAnimation>(m_nobsAnimated, umPntVector));  // after position alignment

    umPntVector.SetDir(Vector2Radian(CalcOrthoVector<IoLine>(m_nobsAnimated, line)));
    AddPhase(make_unique<IoLinesAnimation>(m_nobsAnimated, umPntVector));  // after direction alignment

    umPntVector.Pack(NEURON_RADIUS * 2.0f);
    AddPhase(make_unique<IoLinesAnimation>(m_nobsAnimated, umPntVector));  // after packing

    AddPhase(make_unique<MakeConnAnimation>(move(m_nobsAnimated)));

    m_bAllOk = true;
}

NobType ConnAnimationCommand::determineNobType(UPNobList const & nobs) const
{
    using enum NobType::Value;
    if (nobs.CountInSelection(inputConnector) > 0)
        return undefined;

    if (nobs.CountInSelection(outputConnector) > 0)
        return undefined;

    unsigned int uiNrOfInputLines  { nobs.CountInSelection(inputLine ) };
    unsigned int uiNrOfOutputLines { nobs.CountInSelection(outputLine) };

    if ((uiNrOfInputLines == 0) && (uiNrOfOutputLines == 0))
        return undefined;

    return (uiNrOfInputLines > uiNrOfOutputLines) 
        ? inputLine 
        : outputLine;
}

void ConnAnimationCommand::sortNobsAnimated(MicroMeterLine const & line)
{
    MicroMeterLine orthoLine { line.OrthoLine() };
    sort
    (
        m_nobsAnimated,
        [&](auto & p1, auto & p2) 
        { 
            return PointToLine(orthoLine, p1->GetPos()) < PointToLine(orthoLine, p2->GetPos()); 
        } 
    );
}
