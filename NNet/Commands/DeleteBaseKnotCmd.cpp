// DeletePosNobCmd.cpp
//
// NNetModel

module;

#include <iostream>
#include <memory>

module DeletePosNobCmd;

import SaveCast;
import NNetModel;

using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;

DeletePosNobCmd::DeletePosNobCmd(Nob & nob)
  : m_posNob(*Cast2PosNob(&nob))
{
    m_idOutputLines.Resize(m_posNob.GetNrOfInConns());
    m_idInputLines .Resize(m_posNob.GetNrOfOutConns());

    m_umPos = m_posNob.GetPos();
    m_posNob.Apply2AllOutPipes
    (
        [this](Pipe & pipe) // every outgoing pipe needs a new InputLine as terminator
        { 
            auto upInputLine { make_unique<InputLine>(m_umPos) };
            upInputLine->Select(pipe.IsSelected());
            upInputLine->AddOutgoing(pipe);            // prepare new InputLine as far as possible
            m_inputLines.push_back(move(upInputLine)); // store new InputLine for later
        }                                                  // but do not touch m_pPosNob
    );  // InputLines in m_inputLines have their outgoing pipe set
    m_posNob.Apply2AllInPipes
    (
        [this](Pipe & pipe) // every incoming pipe needs a new OutputLine as terminator
        { 
            auto upOutputLine { make_unique<OutputLine>(m_umPos) };
            upOutputLine->Select(pipe.IsSelected());
            upOutputLine->AddIncoming(pipe);             // prepare new OutputLine as far as possible
            m_outputLines.push_back(move(upOutputLine)); // store new OutputLine for later
        }                                                    // but do not touch m_pPosNob
    );  // OutputLines in m_outputLines have their incoming pipe set
}

void DeletePosNobCmd::Do()
{
    for (size_t i = 0; i < m_outputLines.size(); ++i)
    {
        unique_ptr<OutputLine> & upOutputLine { m_outputLines[i] };
        if (upOutputLine.get())
        {
            Pipe & pipeOut { *upOutputLine->GetPipe() };
            pipeOut.SetEndPnt(upOutputLine.get());
            pipeOut.DislocateEndPoint();
            m_idOutputLines.SetAt(i, m_pNMWI->Push2Model(move(upOutputLine)));
        }
    }
    for (size_t i = 0; i < m_inputLines.size(); ++i)
    {
        unique_ptr<InputLine> & upInputLine { m_inputLines[i] };
        if (upInputLine.get())
        {
            Pipe & pipeIn { *upInputLine->GetPipe() };
            pipeIn.SetStartPnt(upInputLine.get());
            pipeIn.DislocateStartPoint();
            m_idInputLines.SetAt(i, m_pNMWI->Push2Model(move(upInputLine)));
        }
    }
    m_upPosNob = m_pNMWI->RemoveFromModel<PosNob>(m_posNob);
}

void DeletePosNobCmd::Undo()
{
    for (size_t i = m_inputLines.size(); i --> 0;)
    {
        if (auto pInputLine { m_pNMWI->GetNobPtr<InputLine*>(m_idInputLines.Get(i)) })
        {
            m_posNob.AddOutgoing(*pInputLine->GetPipe());
            pInputLine->GetPipe()->SetStartPnt(&m_posNob);
        }
        m_inputLines[i] = m_pNMWI->PopFromModel<InputLine>();
    }
    for (size_t i = m_outputLines.size(); i --> 0;)
    {
        if (auto pOutputLine { m_pNMWI->GetNobPtr<OutputLine*>(m_idOutputLines.Get(i)) })
        {
            m_posNob.AddIncoming(*pOutputLine->GetPipe());
            pOutputLine->GetPipe()->SetEndPnt(&m_posNob);
        }
        m_outputLines[i] = m_pNMWI->PopFromModel<OutputLine>();
    }
    m_upPosNob = m_pNMWI->ReplaceInModel<PosNob>(move(m_upPosNob));
}
