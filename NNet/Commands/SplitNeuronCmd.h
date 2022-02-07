// SplitNeuronCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "NobId.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"

using std::unique_ptr;

class SplitNeuronCmd : public NNetCommand
{
public:
    explicit SplitNeuronCmd(NobId const id)
      : m_neuron(*m_pNMWI->GetNobPtr<Neuron *>(id))
    {
        MicroMeterPnt umPos { m_neuron.GetPos() };
        m_upInputNeuron  = make_unique<InputNeuron >(umPos);
        m_upOutputNeuron = make_unique<OutputNeuron>(umPos);
        m_upInputNeuron ->SetOutgoing(m_neuron);
        m_upOutputNeuron->SetIncoming(m_neuron);
        m_upInputNeuron ->MoveNob((m_neuron.GetFirstOutgoing().GetEndPoint  ()-umPos).ScaledTo(NEURON_RADIUS*2));
        m_upOutputNeuron->MoveNob((m_neuron.GetFirstIncoming().GetStartPoint()-umPos).ScaledTo(NEURON_RADIUS*2));
    }

    ~SplitNeuronCmd() final = default;

    void Do() final
    {
        m_upNeuron = m_pNMWI->RemoveFromModel<Neuron>(m_neuron);
        m_pNMWI->Push2Model(move(m_upInputNeuron));
        m_pNMWI->Push2Model(move(m_upOutputNeuron));
    }

    void Undo() final
    {
        m_upOutputNeuron = m_pNMWI->PopFromModel<OutputNeuron>();
        m_upInputNeuron  = m_pNMWI->PopFromModel<InputNeuron >();
        m_upNeuron->Reconnect();
        m_pNMWI->Restore2Model(move(m_upNeuron));
    }

private:
    Neuron                 & m_neuron;
    unique_ptr<Neuron>       m_upNeuron       { };
    unique_ptr<InputNeuron>  m_upInputNeuron  { };
    unique_ptr<OutputNeuron> m_upOutputNeuron { };
};
