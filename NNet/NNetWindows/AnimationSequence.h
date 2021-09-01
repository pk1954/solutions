// AnimationSequence.h
//
// NNetWindows

#pragma once

#include "SelectionCommand.h"
#include "AnimationCmd.h"

class NNetModelWriterInterface;
class MainWindow;
class Nob;

class AnimationSequence : public SelectionCommand
{
public:
    AnimationSequence(MainWindow &);
    
    virtual void Do  ();
    virtual void Undo();

protected:
    void AddPhase(unique_ptr<AnimationCmd>);

private:
    unsigned int m_uiPhase { 0 };
    MainWindow & m_win;

    vector<unique_ptr<AnimationCmd>> m_phases { };  

    void BlockUI();
    void UnblockUI();

    void doPhase();
    void undoPhase();
};
