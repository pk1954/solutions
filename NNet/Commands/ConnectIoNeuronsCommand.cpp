// ConnectIoNeuronsCommand.cpp
//
// NNetCommands

#pragma once

#include "stdafx.h"
#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "ConnectIoNeuronsCommand.h"

ConnectIoNeuronsCommand::ConnectIoNeuronsCommand
(
    IoNeuron & nobAnimated, 
    IoNeuron & nobTarget
)
  : m_nobTarget(nobTarget),
    m_nobAnimated(nobAnimated)
{
    assert( m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob() );
    assert( m_nobAnimated.GetIoMode() != NobIoMode::internal );
    assert( m_nobTarget  .GetIoMode() != NobIoMode::internal );
    assert( m_nobTarget  .GetIoMode() !== m_nobAnimated.GetIoMode() );

    m_upClosedNeuron = m_nobAnimated.IsInputNob()
        ? make_unique<Neuron>(m_nobTarget.GetPos(), m_nobAnimated, m_nobTarget  )
        : make_unique<Neuron>(m_nobTarget.GetPos(), m_nobTarget,   m_nobAnimated);
}

void ConnectIoNeuronsCommand::Do(NNetModelWriterInterface& nmwi)
{
    m_upClosedNeuron->SetParentPointers();
    NobId id = nmwi.GetUPNobs().Push(move(m_upClosedNeuron));
    m_upNobAnimated = nmwi.RemoveFromModel<IoNeuron>(m_nobAnimated);
    m_upNobTarget   = nmwi.RemoveFromModel<IoNeuron>(m_nobTarget);
    nmwi.GetNobPtr<Neuron *>(id)->Reconnect();
}

void ConnectIoNeuronsCommand::Undo(NNetModelWriterInterface& nmwi)
{
    m_upClosedNeuron = nmwi.GetUPNobs().Pop<Neuron>();
    m_upClosedNeuron->ClearParentPointers();
    m_upNobAnimated->Reconnect();
    m_upNobTarget  ->Reconnect();
    m_upNobAnimated = nmwi.ReplaceInModel<IoNeuron,IoNeuron>(move(m_upNobAnimated));
    m_upNobTarget   = nmwi.ReplaceInModel<IoNeuron,IoNeuron>(move(m_upNobTarget));
}
