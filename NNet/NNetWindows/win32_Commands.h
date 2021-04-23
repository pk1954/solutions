// win32_Commands.h
//
// NNetWindows

#pragma once

#include <string>

class Command;
class CommandStack;
class NNetModelCommands;
class RootWindow;

using std::wcout;
using std::wostream;

class WinCommands
{
public:
    void Initialize( CommandStack * const, NNetModelCommands * const );

    void AlignDirection(RootWindow * const);
    void AlignShapes   (RootWindow * const);
    void PackShapes    (RootWindow * const);

private:

    bool       IsTraceOn  () const { return m_bTrace; }
    wostream & TraceStream()       { return wcout; }

    bool                m_bTrace    { true };
    CommandStack      * m_pCmdStack { nullptr };
    NNetModelCommands * m_pCommands { nullptr };
};