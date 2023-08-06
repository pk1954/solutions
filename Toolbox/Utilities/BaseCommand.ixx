// BaseCommand.ixx
//
// Utilities

module;

#include <memory>
#include <functional>
#include <vector>

export module BaseCommand;

import SaveCast;

using std::function;
using std::vector;
using std::unique_ptr;

export class BaseCommand
{
public:

    virtual void UpdateUI() = 0;

    void TargetReached()
    {
        if (m_targetReachedFunc)
            (m_targetReachedFunc)();
    }

    virtual void Do()
    {
        m_uiPhase = 0;
        doPhase();
    }

    virtual void Undo()
    {
        m_uiPhase = Cast2UnsignedInt(m_phases.size());
        undoPhase();
    }

protected:

    void AddPhase(unique_ptr<BaseCommand> upCmd)
    {
        m_phases.push_back(move(upCmd));
    }

    virtual void BlockUI()   const = 0;
    virtual void UnblockUI() const = 0;

private:

    void doPhase() // runs in UI thread
    {
        if (!m_phases.empty())
        {
            if (m_uiPhase == 0)
                BlockUI();
            if (m_uiPhase < m_phases.size())
            {
                BaseCommand* const pAnimCmd { m_phases[m_uiPhase++].get() };
                pAnimCmd->m_targetReachedFunc = [&]() { doPhase(); };
                pAnimCmd->Do();
            }
            else
                UnblockUI();
        }
        UpdateUI();
    }

    void undoPhase() // runs in UI thread
    {
        if (m_uiPhase >= m_phases.size())
            BlockUI();
        if (m_uiPhase > 0)
        {
            BaseCommand& animCmd { *m_phases[--m_uiPhase] };
            animCmd.m_targetReachedFunc = [&]() { undoPhase(); };
            animCmd.Undo();
        }
        else
            UnblockUI();
        UpdateUI();
    }

    function<void()>                m_targetReachedFunc { nullptr };
    vector<unique_ptr<BaseCommand>> m_phases            { };
    unsigned int                    m_uiPhase           { 0 };
};
