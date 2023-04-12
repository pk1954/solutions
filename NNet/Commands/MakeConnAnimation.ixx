// MakeConnAnimation.ixx
//
// Commands

module;

#include <vector>
#include <memory>

export module NNetCommands:MakeConnAnimation;

import :NNetCommand;
import NNetModel;

using std::vector;
using std::unique_ptr;
using std::make_unique;

export class MakeConnAnimation : public NNetCommand
{
public:
    explicit MakeConnAnimation(vector<IoLine*>&& list)
    {
        if (list.front()->IsInputLine())
            m_upIoConnector = make_unique<InputConnector>(move(list));
        else
            m_upIoConnector = make_unique<OutputConnector>(move(list));
    }

    void Do() final
    {
        m_pNMWI->DeselectAllNobs();
        m_upIoConnector->ConnectIoLines();
        m_pNMWI->Push2Model(move(m_upIoConnector));
        (m_targetReachedFunc)();
    }

    void Undo() final
    {
        m_upIoConnector = m_pNMWI->PopFromModel<IoConnector>();
        m_upIoConnector->DisconnectIoLines();
        (m_targetReachedFunc)();
    }

private:
        unique_ptr<IoConnector> m_upIoConnector{};
};