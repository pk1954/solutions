// SplitClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "NobId.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "ClosedConnector.h"

using std::unique_ptr;

/// <summary>
/// Not working !
/// </summary>

class SplitClosedConnCmd : public Command
{
public:
    SplitClosedConnCmd(NobId const id)
      : m_closedConnector(*m_pNMWI->GetNobPtr<ClosedConnector *>(id))
    {
        m_upInputConnector  = make_unique<InputConnector>();
        m_upOutputConnector = make_unique<OutputConnector>();
        m_size = m_closedConnector.Size();
        m_closedConnector.Apply2All
        (
            [&](Neuron & neuron)
            {
                unique_ptr<InputNeuron > upInputNeuron  { make_unique<InputNeuron >(neuron) };
                m_upInputConnector->Push(upInputNeuron.get());
                m_upInputNeurons.push_back(move(upInputNeuron));
                unique_ptr<OutputNeuron> upOutputNeuron { make_unique<OutputNeuron>(neuron) };
                m_upOutputConnector->Push(upOutputNeuron.get());
                m_upOutputNeurons.push_back(move(upOutputNeuron));
            }
        );
        m_upInputConnector ->SetParentPointers();
        m_upOutputConnector->SetParentPointers();
        MicroMeterLine line     { m_closedConnector.CalcMaxDistLine() };
        MicroMeterPnt  umPntDir { m_closedConnector.CalcOrthoVector(line).ScaledTo(NEURON_RADIUS*2.0f) };
        Radian         radDir   { Vector2Radian(umPntDir) };
        Degrees        degDir   { Radian2Degrees(radDir) };
        m_upInputConnector ->SetDir(radDir);
        m_upOutputConnector->SetDir(radDir);
        m_upInputConnector ->MoveNob(umPntDir);
        m_upOutputConnector->MoveNob(-umPntDir);
    }

    ~SplitClosedConnCmd() {}

    virtual void Do()
    {
        m_closedConnector.Apply2All([&](Neuron const &n){ m_upNeurons.push_back(m_pNMWI->RemoveFromModel<Neuron>(n)); });
        m_upClosedConnector = m_pNMWI->RemoveFromModel<ClosedConnector>(m_closedConnector);
        m_upClosedConnector->ClearParentPointers();

        for (size_t i = 0; i < m_size; ++i)
        { 
            m_pNMWI->Push2Model(move(m_upInputNeurons.back()));
            m_upInputNeurons.pop_back();
            m_pNMWI->Push2Model(move(m_upOutputNeurons.back()));
            m_upOutputNeurons.pop_back();
        }
        m_pNMWI->Push2Model(move(m_upInputConnector));
        m_pNMWI->Push2Model(move(m_upOutputConnector));
    }

    virtual void Undo()
    {
        m_upOutputConnector = m_pNMWI->PopFromModel<OutputConnector>();
        m_upInputConnector  = m_pNMWI->PopFromModel<InputConnector>();
        for ( size_t i = 0; i < m_size; ++i )
        {
            m_upOutputNeurons[i] = m_pNMWI->PopFromModel<IoNeuron>();
            m_upInputNeurons [i] = m_pNMWI->PopFromModel<IoNeuron>();
        }

        m_upClosedConnector->SetParentPointers();
        m_upClosedConnector = m_pNMWI->ReplaceInModel<ClosedConnector,ClosedConnector>(move(m_upClosedConnector));
        for (auto & it : m_upNeurons) 
            it = m_pNMWI->ReplaceInModel<Neuron,Neuron>(move(it));
    }

private:

    size_t                       m_size;
    ClosedConnector            & m_closedConnector;

    // take ownership of ClosedConnector and Neurons between Do and Undo
    unique_ptr<ClosedConnector>  m_upClosedConnector {};
    vector<unique_ptr<Neuron>>   m_upNeurons;              

    // take ownership of IoConnectors and IoNeurons between Undo and redo
    unique_ptr<InputConnector>   m_upInputConnector  {};  
    unique_ptr<OutputConnector>  m_upOutputConnector {};  
    vector<unique_ptr<IoNeuron>> m_upInputNeurons;              
    vector<unique_ptr<IoNeuron>> m_upOutputNeurons;              
};
