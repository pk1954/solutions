// win32_command.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_rootWindow.h"
#include "win32_command.h"

void Command::UpdateUI() 
{ 
    m_pWin->Notify(false); 
};

void Command::CallUI(bool const bTargetReached)
{
    PostMessage
    (
        m_pWin->GetWindowHandle(), 
        WM_APP_UI_CALL, 
        static_cast<WPARAM>(bTargetReached), 
        reinterpret_cast<LPARAM>(this)
    );
}

void Command::DoCall(WPARAM const wParam, LPARAM const lParam) 
{
    Command * const pAnimCmd       { reinterpret_cast<Command * const>(lParam) };
    bool           const bTargetReached { static_cast<bool const>(wParam) };
    pAnimCmd->UpdateUI();
    if (bTargetReached && pAnimCmd->m_targetReachedFunc)
        (pAnimCmd->m_targetReachedFunc)(); 
};

void Command::doPhase() // runs in UI thread
{
    //wcout << L'#' << __FUNCDNAME__ << L" phase(" << m_uiPhase << L'/' << m_phases.size() << L')' << endl;
    if (m_uiPhase == 0)
        blockUI();
    if (m_uiPhase < m_phases.size())
    {
        Command & animCmd { * m_phases[m_uiPhase++] };
        animCmd.m_targetReachedFunc = [&](){ doPhase(); };
        animCmd.Do();
    }
    else
        unblockUI();
    UpdateUI();
}

void Command::undoPhase() // runs in UI thread
{
    //wcout << L'#' << __FUNCDNAME__ << L" phase(" << m_uiPhase << L'/' << m_phases.size() << L')' << endl;
    if (m_uiPhase >= m_phases.size())
        blockUI();
    if (m_uiPhase > 0)
    {
        Command & animCmd { * m_phases[--m_uiPhase] };
        animCmd.m_targetReachedFunc = [&](){ undoPhase(); };
        animCmd.Undo();
    }
    else
        unblockUI();
    UpdateUI();
}
void Command::Do()
{
    m_uiPhase = 0;
    doPhase();
}

void Command::Undo()
{
    m_uiPhase = Cast2Int(m_phases.size());
    undoPhase();
}

void Command::AddPhase(unique_ptr<Command> upCmd)
{
    m_phases.push_back(move(upCmd));
}

void Command::blockUI()   
{ 
    m_pWin->SendCommand2Application(IDM_BLOCK_UI, true);  
};

void Command::unblockUI()
{ 
    m_pWin->SendCommand2Application(IDM_BLOCK_UI, false); 
};
