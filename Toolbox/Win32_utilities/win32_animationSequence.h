// win32_animationSequence.h
//
// Win32_utilities

#pragma once

#include "BaseCommand.h"
#include "win32_animationCmd.h"

class RootWindow;

class AnimationSequence : public BaseCommand
{
public:
    AnimationSequence(RootWindow &);
    
    virtual void Do  ();
    virtual void Undo();

    void DoPhase();
    void UndoPhase();

protected:
    void AddPhase(unique_ptr<AnimationCmd>);

private:
    unsigned int m_uiPhase { 0 };
    RootWindow & m_win;

    vector<unique_ptr<AnimationCmd>> m_phases { };  

    void BlockUI();
    void UnblockUI();
};
