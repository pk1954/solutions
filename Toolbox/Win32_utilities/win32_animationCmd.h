// win32_animationCmd.h
//
// Win32_utilities

#pragma once

#include <vector>
#include "BaseCommand.h"

using std::vector;
using std::function;
using std::unique_ptr;

class RootWindow;

class AnimationCmd : public BaseCommand
{
public:
    virtual void Do  ();
    virtual void Undo();
    virtual void UpdateUI();

    void CallUI(bool const);

    static void DoCall(WPARAM const, LPARAM const); 

    static void Initialize(RootWindow * const pWin)
    {
        m_pWin = pWin;
    }

    inline static RootWindow * m_pWin { nullptr };

protected:
    void AddPhase(unique_ptr<AnimationCmd>);

    function<void()> m_targetReachedFunc { nullptr };

private:

    vector<unique_ptr<AnimationCmd>> m_phases  { };  
    unsigned int                     m_uiPhase { 0 };

    void doPhase();
    void undoPhase();

    void blockUI();
    void unblockUI();
};