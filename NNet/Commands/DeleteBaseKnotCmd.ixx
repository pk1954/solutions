// DeletePosNobCmd.ixx
//
// Commands

module;

#include <vector>
#include <memory>

export module DeletePosNobCmd;

import Types;
import NNetCommand;
import NNetModel;

using std::vector;
using std::unique_ptr;

export class DeletePosNobCmd : public NNetCommand
{
public:
    DeletePosNobCmd(Nob &);

    ~DeletePosNobCmd() final = default;

    void Do  () final;
    void Undo() final;

private:
    PosNob                     & m_posNob;
    unique_ptr<PosNob>           m_upPosNob;
    MicroMeterPnt                  m_umPos         {};
    vector<unique_ptr<OutputLine>> m_outputLines   {};
    vector<unique_ptr<InputLine>>  m_inputLines    {};
    NobIdList                      m_idOutputLines {};
    NobIdList                      m_idInputLines  {};
};
