// AnimationCmd.h
//
// NNetWindows

#pragma once

class MainWindow;

using std::function;

class AnimationCmd
{
public:
    AnimationCmd(MainWindow & win)
        : m_win(win)
    {}

    virtual void Do(function<void()> const & func)
    {
        m_targetReachedFunc = func;
    };
    virtual void Undo(function<void()> const & func)
    {
        m_targetReachedFunc = func;
    };

protected:
    MainWindow     & m_win;
    function<void()> m_targetReachedFunc { nullptr };
};