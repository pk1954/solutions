// PlugIoConnectors.h
//
// NNetWindows

#pragma once

#include <vector>
#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "IoConnector.h"
#include "AnimationCmd.h"

using std::vector;
using std::unique_ptr;

class MainWindow;

class PlugIoConnectors : public AnimationCmd
{
public:
    PlugIoConnectors
    (
        NNetModelWriterInterface & nmwi,
        IoConnector              & conn1, 
        IoConnector              & conn2,
        MainWindow               & win
   )
      : AnimationCmd(win),
        m_nmwi(nmwi),
        m_inputConnector (conn1.IsInputNob () ? conn1 : conn2),
        m_outputConnector(conn1.IsOutputNob() ? conn1 : conn2)
    {
        assert(m_inputConnector.Size() == m_outputConnector.Size());

        m_size              = m_inputConnector.Size();
        for (size_t i = 0; i < m_size; ++i)
        {
            MicroMeterPnt const umPos    { m_inputConnector.GetElem(i).GetPos() };
            unique_ptr<Neuron>  upNeuron { make_unique<Neuron>(umPos) };
            upNeuron->SetIncoming(m_outputConnector.GetElem(i));
            upNeuron->SetOutgoing(m_inputConnector .GetElem(i));
            m_upNeurons.push_back(move(upNeuron));
        }
        m_upOutputNeurons.resize(m_size);
        m_upInputNeurons .resize(m_size);
        nmwi.CheckModel();
    }

    virtual void DoAnimation(function<void()> const & targetReachedFunc)
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_nmwi.Push2Model(move(m_upNeurons.back()));
            m_upNeurons.pop_back();
        }

        m_upOutputConnector = m_nmwi.RemoveFromModel<IoConnector>(m_outputConnector);
        m_upInputConnector  = m_nmwi.RemoveFromModel<IoConnector>(m_inputConnector );
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upOutputNeurons[i] = m_nmwi.RemoveFromModel<IoNeuron>(m_outputConnector.GetElem(i));
            m_upInputNeurons [i] = m_nmwi.RemoveFromModel<IoNeuron>(m_inputConnector .GetElem(i));
        }

        if (targetReachedFunc)
            (targetReachedFunc)();
        m_nmwi.CheckModel();
    }

    virtual void UndoAnimation(function<void()> const & targetReachedFunc)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_upNeurons.push_back(m_nmwi.PopFromModel<Neuron>());

        m_upOutputConnector = m_nmwi.ReplaceInModel<IoConnector, IoConnector>(move(m_upOutputConnector));
        m_upInputConnector  = m_nmwi.ReplaceInModel<IoConnector, IoConnector>(move(m_upInputConnector ));
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upOutputNeurons[i] = m_nmwi.ReplaceInModel<IoNeuron, IoNeuron>(move(m_upOutputNeurons[i]));
            m_upInputNeurons [i] = m_nmwi.ReplaceInModel<IoNeuron, IoNeuron>(move(m_upInputNeurons [i]));
        }

        if (targetReachedFunc)
            (targetReachedFunc)();
        m_nmwi.CheckModel();
    }

private:

    NNetModelWriterInterface     & m_nmwi;
    size_t                         m_size;

    IoConnector            const & m_inputConnector;
    IoConnector            const & m_outputConnector;

    vector<unique_ptr<Neuron>>   m_upNeurons;              

    // take ownership of IoConnectors and IoNeurons between Do and Undo
    unique_ptr<IoConnector>      m_upInputConnector;
    unique_ptr<IoConnector>      m_upOutputConnector;
    vector<unique_ptr<IoNeuron>> m_upInputNeurons;              
    vector<unique_ptr<IoNeuron>> m_upOutputNeurons;              
};
