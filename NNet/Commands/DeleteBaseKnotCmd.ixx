// DeleteBaseKnotCmd.ixx
//
// Commands

module;

#include <vector>
#include "Nob.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "BaseKnot.h"

export module DeleteBaseKnotCmd;

import PixelTypes;
import NobIdList;
import NNetCommand;

using std::vector;
using std::unique_ptr;

export class DeleteBaseKnotCmd : public NNetCommand
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
