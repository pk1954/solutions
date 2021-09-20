// AnimationSequence.h
//
// NNetWindows

#pragma once

#include "Command.h"
#include "win32_animationCmd.h"

class RootWindow;

class AnimationSequence : public Command
{
public:
    AnimationSequence(RootWindow &);
    
    virtual void Do  ();
    virtual void Undo();

protected:
    void AddPhase(unique_ptr<AnimationCmd>);

private:
    unsigned int m_uiPhase { 0 };
    RootWindow & m_win;

    vector<unique_ptr<AnimationCmd>> m_phases { };  

    void BlockUI();
    void UnblockUI();

    void doPhase();
    void undoPhase();
};
