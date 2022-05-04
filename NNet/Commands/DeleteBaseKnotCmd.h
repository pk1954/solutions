// DeleteBaseKnotCmd.h
//
// Commands

#pragma once

#include <vector>
#include "NNetCommand.h"
#include "NobIdList.h"

class InputLine;
class OutputLine;
class BaseKnot;

using std::vector;
using std::unique_ptr;

class DeleteBaseKnotCmd : public NNetCommand
{
public:
    DeleteBaseKnotCmd(Nob &);

    ~DeleteBaseKnotCmd() final = default;

    void Do  () final;
    void Undo() final;

private:
    BaseKnot                     & m_baseKnot;
    unique_ptr<BaseKnot>           m_upBaseKnot;
    MicroMeterPnt                  m_umPos         {};
    vector<unique_ptr<OutputLine>> m_outputLines   {};
    vector<unique_ptr<InputLine>>  m_inputLines    {};
    NobIdList                      m_idOutputLines {};
    NobIdList                      m_idInputLines  {};
};
