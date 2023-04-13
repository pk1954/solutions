// SplitNeuronCmd.ixx
//
// Commands

module;

#include <memory>
#include <cassert>
#include <iostream>

export module NNetCommands:SplitNeuronCmd;

import Types;
import :NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class SplitNeuronCmd : public NNetCommand
{
public:
    explicit SplitNeuronCmd(NobId const id)
        : m_neuron(*m_pNMWI->GetNobPtr<Neuron *>(id))
    {
        assert(m_neuron.GetNrOfInConns() == 1);
        MicroMeterPnt umPos { m_neuron.GetPos() };
        m_upInputLine  = make_unique<InputLine >(umPos);
        m_upOutputLine = make_unique<OutputLine>(umPos);
        m_upInputLine ->AddOutgoing(m_neuron.GetAxon());
        m_upOutputLine->AddIncoming(m_neuron.GetFirstIncoming());
        m_upInputLine ->MoveNob((m_neuron.GetAxon         ()->GetEndPoint  ()-umPos).ScaledTo(NEURON_RADIUS*2));
        m_upOutputLine->MoveNob((m_neuron.GetFirstIncoming()->GetStartPoint()-umPos).ScaledTo(NEURON_RADIUS*2));
    }

    ~SplitNeuronCmd() final = default;

    void Do() final
    {
        m_upNeuron = m_pNMWI->RemoveFromModel<Neuron>(m_neuron);
        m_pNMWI->Push2Model(move(m_upInputLine));
        m_pNMWI->Push2Model(move(m_upOutputLine));
        m_pSound->Play(L"UNLOCK_SOUND");
    }

    void Undo() final
    {
        m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
        m_upInputLine  = m_pNMWI->PopFromModel<InputLine >();
        m_upNeuron->Reconnect();
        m_pNMWI->Restore2Model(move(m_upNeuron));
        m_pSound->Play(L"SNAP_IN_SOUND");
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(NobId nobId)
    {
        if (IsTraceOn())
            TraceStream() << NAME << nobId.GetValue() << endl;
        PushCommand(make_unique<SplitNeuronCmd>(nobId));
    }

private:

    inline static const wstring NAME { L"SplitNeuron" };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            SplitNeuronCmd::Push(ScrReadNobId(script));
        }
    } m_wrapper;

    Neuron               & m_neuron;
    unique_ptr<Neuron>     m_upNeuron     { };
    unique_ptr<InputLine>  m_upInputLine  { };
    unique_ptr<OutputLine> m_upOutputLine { };
};
