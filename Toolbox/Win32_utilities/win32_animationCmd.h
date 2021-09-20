// win32_animationCmd.h
//
// Win32_utilities

#pragma once

#include "win32_util_resource.h"
#include "win32_animation.h"

using std::function;

class RootWindow;
class NNetModelWriterInterface;

class AnimationCmd
{
public:
    AnimationCmd();

    virtual void DoAnimation  () = 0;
    virtual void UndoAnimation() = 0;

    virtual void UpdateUI() 
    { 
        m_pWin->Notify(false); 
    };

    static void DoCall(WPARAM const, LPARAM const); 

    static void Initialize(RootWindow * const pWin)
    {
        m_pWin = pWin;
    }

    void SetTargetReachedFunc(function<void()> const & func) { m_targetReachedFunc = func; }

protected:
  
    APP_PROC m_applicationFunc;

    function<void()> m_targetReachedFunc { nullptr };

private:
    inline static RootWindow * m_pWin { nullptr };

};