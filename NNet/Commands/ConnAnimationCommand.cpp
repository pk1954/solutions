// ConnAnimationCommand.cpp
//
// Command

module;

#include <cassert>
#include <algorithm>
#include <memory>
#include <vector>

module NNetCommands:ConnAnimationCommand;

import Types;
import NNetModel;
import :LockDirectionCmd;
import :IoLinesAnimation;
import :MakeConnAnimation;

using std::make_unique;
using std::ranges::sort;

ConnAnimationCommand::ConnAnimationCommand
(
    NobId const id1,
    NobId const id2
)
  : m_id1(id1),
    m_id2(id2)
{
    m_nobsAnimated.clear();
    add2nobsAnimated(id1);
    add2nobsAnimated(id2);

    MicroMeterLine umLine { CalcMaxDistLine<IoLine>(m_nobsAnimated) };
    if (umLine.IsZero())
        return;

    sortNobsAnimated(umLine);

    MicroMeterPntVector umPntVector(m_nobsAnimated);  // before animation

    AddPhase(make_unique<LockDirectionCmd>(m_nobsAnimated));

    umPntVector.Align(umLine);
    AddPhase(make_unique<IoLinesAnimation>(m_nobsAnimated, umPntVector));  // after position alignment

    umPntVector.SetDirVector(::CalcOrthoVector(m_nobsAnimated));
    AddPhase(make_unique<IoLinesAnimation>(m_nobsAnimated, umPntVector));  // after direction alignment

    umPntVector.Pack(NEURON_RADIUS * 2.0f);
    AddPhase(make_unique<IoLinesAnimation>(m_nobsAnimated, umPntVector));  // after packing

    AddPhase(make_unique<MakeConnAnimation>(move(m_nobsAnimated)));
}

void ConnAnimationCommand::add2nobsAnimated(NobId const idNob)
{
    Nob* pNob { m_pNMWI->GetNob(idNob) };
    if (pNob->IsIoConnector())
    {
        static_cast<IoConnector*>(pNob)->Apply2All([this](IoLine& io) { m_nobsAnimated.push_back(&io); });
    }
    else if (pNob->IsIoLine())
    {
        m_nobsAnimated.push_back(static_cast<InputLine*>(pNob));
    }
    else
        assert(false);
}

void ConnAnimationCommand::sortNobsAnimated(MicroMeterLine const & umLine)
{
    MicroMeterLine orthoLine { umLine.OrthoLine() };
    sort
    (
        m_nobsAnimated,
        [&orthoLine](auto p1, auto p2) 
        { 
            return PointToLine(orthoLine, p1->GetPos()) < PointToLine(orthoLine, p2->GetPos());
        } 
    );
}
