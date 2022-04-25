// DeleteBaseKnotCmd.cpp
//
// NNetModel

#include "stdafx.h"
#include "DeleteBaseKnotCmd.h"

using std::wcout;
using std::endl;
using std::make_unique;

DeleteBaseKnotCmd::DeleteBaseKnotCmd(Nob & nob)
  : m_baseKnot(*Cast2BaseKnot(&nob))
{
    m_idOutputLines.Resize(m_baseKnot.GetNrOfInConns());
    m_idInputLines .Resize(m_baseKnot.GetNrOfOutConns());

    m_umPos = m_baseKnot.GetPos();
    m_baseKnot.Apply2AllOutPipes
    (
        [this](Pipe & pipe) // every outgoing pipe needs a new InputLine as terminator
        { 
            auto upInputLine { make_unique<InputLine>(m_pNMWI->StdSigGen(), m_umPos) };
            upInputLine->Select(pipe.IsSelected());
            upInputLine->AddOutgoing(pipe);            // prepare new InputLine as far as possible
            m_inputLines.push_back(move(upInputLine)); // store new InputLine for later
        }                                                  // but do not touch m_pBaseKnot
    );  // InputLines in m_inputLines have their outgoing pipe set
    m_baseKnot.Apply2AllInPipes
    (
        [this](Pipe & pipe) // every incoming pipe needs a new OutputLine as terminator
        { 
            auto upOutputLine { make_unique<OutputLine>(m_umPos) };
            upOutputLine->Select(pipe.IsSelected());
            upOutputLine->AddIncoming(pipe);             // prepare new OutputLine as far as possible
            m_outputLines.push_back(move(upOutputLine)); // store new OutputLine for later
        }                                                    // but do not touch m_pBaseKnot
    );  // OutputLines in m_outputLines have their incoming pipe set
}

void DeleteBaseKnotCmd::Do()
{
    for (size_t i = 0; i < m_outputLines.size(); ++i)
    {
        unique_ptr<OutputLine> & upOutputLine { m_outputLines[i] };
        if (upOutputLine.get())
        {
            Pipe & pipeOut { upOutputLine->GetFirstIncoming() };
            pipeOut.SetEndKnot(upOutputLine.get());
            pipeOut.DislocateEndPoint();
            m_idOutputLines.SetAt(i, m_pNMWI->Push2Model(move(upOutputLine)));
        }
    }
    for (size_t i = 0; i < m_inputLines.size(); ++i)
    {
        unique_ptr<InputLine> & upInputLine { m_inputLines[i] };
        if (upInputLine.get())
        {
            Pipe & pipeIn { upInputLine->GetFirstOutgoing() };
            pipeIn.SetStartKnot(upInputLine.get());
            pipeIn.DislocateStartPoint();
            m_idInputLines.SetAt(i, m_pNMWI->Push2Model(move(upInputLine)));
        }
    }
    m_upBaseKnot = m_pNMWI->RemoveFromModel<BaseKnot>(m_baseKnot);
}

void DeleteBaseKnotCmd::Undo()
{
    for (size_t i = m_inputLines.size(); i --> 0;)
    {
        if (auto pInputLine { m_pNMWI->GetNobPtr<InputLine *>(m_idInputLines.Get(i)) })
            ConnectOutgoing(pInputLine->GetFirstOutgoing(), m_baseKnot);
        m_inputLines[i] = m_pNMWI->PopFromModel<InputLine>();
    }
    for (size_t i = m_outputLines.size(); i --> 0;)
    {
        if (auto pOutputLine { m_pNMWI->GetNobPtr<OutputLine *>(m_idOutputLines.Get(i)) })
            ConnectIncoming(pOutputLine->GetFirstIncoming(), m_baseKnot);
        m_outputLines[i] = m_pNMWI->PopFromModel<OutputLine>();
    }
    m_upBaseKnot = m_pNMWI->ReplaceInModel<BaseKnot>(move(m_upBaseKnot));
}
