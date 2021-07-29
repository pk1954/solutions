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

class SplitClosedConnCmd : public Command
{
public:
    SplitClosedConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const id
    )
      : m_closedConnector(*nmwi.GetNobPtr<ClosedConnector *>(id))
    {
        m_upInputConnector  = make_unique<InputConnector>();
        m_upOutputConnector = make_unique<OutputConnector>();
        for (Neuron * pNeuron : m_closedConnector.GetNeurons())
        {
            unique_ptr<InputNeuron > upInputNeuron  { make_unique<InputNeuron >(*pNeuron) };
            m_upInputConnector->Push(upInputNeuron.get());
            m_upInputNeurons.push_back(move(upInputNeuron));
            unique_ptr<OutputNeuron> upOutputNeuron { make_unique<OutputNeuron>(*pNeuron) };
            m_upOutputConnector->Push(upOutputNeuron.get());
            m_upOutputNeurons.push_back(move(upOutputNeuron));
        }
        m_upInputConnector ->SetParentPointers();
        m_upOutputConnector->SetParentPointers();
        MicroMeterLine line     { m_closedConnector.CalcMaxDistLine() };
        MicroMeterPnt  umPntDir { m_closedConnector.CalcOrthoVector(line).ScaledTo(NEURON_RADIUS*2.0f) };
        Radian         radDir   { Vector2Radian(umPntDir) };
        Degrees        degDir   { Radian2Degrees(radDir) };
        m_upInputConnector ->SetDir(radDir);
        m_upOutputConnector->SetDir(radDir);
        m_upInputConnector ->MoveNob( umPntDir);
        m_upOutputConnector->MoveNob(-umPntDir);
    }

    ~SplitClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_closedConnector.Apply2All([&](Neuron const &n){ m_upNeurons.push_back(nmwi.RemoveFromModel<Neuron>(n)); });
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_closedConnector);
        m_upClosedConnector->ClearParentPointers();

        for (auto & it : m_upInputNeurons ) { nmwi.Push2Model(move(it)); }
        for (auto & it : m_upOutputNeurons) { nmwi.Push2Model(move(it)); }
        nmwi.Push2Model(move(m_upInputConnector));
        nmwi.Push2Model(move(m_upOutputConnector));
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        m_upOutputConnector = nmwi.PopFromModel<OutputConnector>();
        m_upInputConnector  = nmwi.PopFromModel<InputConnector>();
        for (auto & it : m_upOutputNeurons) { it = nmwi.PopFromModel<IoNeuron>(); }
        for (auto & it : m_upInputNeurons ) { it = nmwi.PopFromModel<IoNeuron>(); }

        m_upClosedConnector->SetParentPointers();
        nmwi.Restore2Model(move(m_upClosedConnector));
        for (auto & it : m_upNeurons) { nmwi.Restore2Model(move(it)); }
    }

private:

    ClosedConnector            & m_closedConnector;

    unique_ptr<ClosedConnector>  m_upClosedConnector {};
    vector<unique_ptr<Neuron>>   m_upNeurons;              

    unique_ptr<InputConnector>   m_upInputConnector  {};  
    unique_ptr<OutputConnector>  m_upOutputConnector {};  
    vector<unique_ptr<IoNeuron>> m_upInputNeurons;              
    vector<unique_ptr<IoNeuron>> m_upOutputNeurons;              
};
