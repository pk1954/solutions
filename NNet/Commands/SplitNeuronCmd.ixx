// SplitNeuronCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "InputLine.h"

export module SplitNeuronCmd;

import NobId;
import Types;
import NNetCommand;
import OutputLine;
import Neuron;

using std::unique_ptr;

export class SplitNeuronCmd : public NNetCommand
{
public:
    explicit SplitNeuronCmd(NobId const id)
        : m_neuron(*m_pNMWI->GetNobPtr<Neuron *>(id))
    {
        MicroMeterPnt umPos { m_neuron.GetPos() };
        m_upInputLine  = make_unique<InputLine >(m_pNMWI->StdSigGen(), umPos);
        m_upOutputLine = make_unique<OutputLine>(umPos);
        m_upInputLine ->SetOutgoing(m_neuron);
        m_upOutputLine->SetIncoming(m_neuron);
        m_upInputLine ->MoveNob((m_neuron.GetFirstOutgoing().GetEndPoint  ()-umPos).ScaledTo(NEURON_RADIUS*2));
        m_upOutputLine->MoveNob((m_neuron.GetFirstIncoming().GetStartPoint()-umPos).ScaledTo(NEURON_RADIUS*2));
    }

    ~SplitNeuronCmd() final = default;

    void Do() final
    {
        m_upNeuron = m_pNMWI->RemoveFromModel<Neuron>(m_neuron);
        m_pNMWI->Push2Model(move(m_upInputLine));
        m_pNMWI->Push2Model(move(m_upOutputLine));
    }

    void Undo() final
    {
        m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
        m_upInputLine  = m_pNMWI->PopFromModel<InputLine >();
        m_upNeuron->Reconnect();
        m_pNMWI->Restore2Model(move(m_upNeuron));
    }

private:
    Neuron               & m_neuron;
    unique_ptr<Neuron>     m_upNeuron     { };
    unique_ptr<InputLine>  m_upInputLine  { };
    unique_ptr<OutputLine> m_upOutputLine { };
};
