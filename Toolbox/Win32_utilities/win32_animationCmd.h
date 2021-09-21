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
    AnimationCmd();

    virtual void Do  ();
    virtual void Undo();
    virtual void UpdateUI();

    static void DoCall(WPARAM const, LPARAM const); 

    static void Initialize(RootWindow * const pWin)
    {
        m_pWin = pWin;
    }

protected:
    void AddPhase(unique_ptr<AnimationCmd>);

    function<void()>           m_targetReachedFunc { nullptr };
    function<void(bool const)> m_applicationFunc   { nullptr };

private:
    inline static RootWindow * m_pWin { nullptr };

    vector<unique_ptr<AnimationCmd>> m_phases  { };  
    unsigned int                     m_uiPhase { 0 };

    void doPhase();
    void undoPhase();

    void blockUI();
    void unblockUI();
};