// DiscBaseKnotCmd.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "NobId.h"
#include "NobIdList.h"
#include "BaseKnot.h"
#include "Knot.h"

using std::vector;
using std::unique_ptr;

// DisconnectBaseKnot and DeleteBaseKnot are nearly identical.
// For Delete... functionality create DisconnectBaseKnot 
// with bool parameter set to true.

class DiscBaseKnotCmd : public NNetCommand
{
public:
    DiscBaseKnotCmd(Nob &, bool const);

    ~DiscBaseKnotCmd() {}

    virtual void Do  ();
    virtual void Undo();

private:
    BaseKnot               & m_baseKnot;
    unique_ptr<BaseKnot>     m_upBaseKnot;
    MicroMeterPnt            m_umPos       {};
    vector<unique_ptr<Knot>> m_startKnots  {};
    vector<unique_ptr<Knot>> m_endKnots    {};
    NobIdList                m_idStartKnots{};
    NobIdList                m_idEndKnots  {};
    bool                     m_bDelete; // true: delete BaseKnot, false: disconnect only
};
