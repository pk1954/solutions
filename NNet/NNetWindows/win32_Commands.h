// win32_Commands.h
//
// NNetWindows
//
// Handle commands with animations

#pragma once

#include <string>
#include "ConnAnimationCommand.h"

class Command;
class CommandStack;
class NobIdList;
class NNetModelCommands;
class NNetModelReaderInterface;
class NNetModelWriterInterface;
class MainWindow;

using std::wcout;
using std::wostream;
using std::unique_ptr;

class WinCommands
{
public:
    void Initialize
    ( 
        CommandStack             * const, 
        NNetModelCommands        * const,
        NNetModelReaderInterface * const,
        NNetModelWriterInterface * const
    );

    void Connect      (NobId const, NobId const, MainWindow &);
    bool MakeIoConnector(MainWindow &);

    NNetModelWriterInterface & GetNMWI() { return * m_pNMWI; }

private:

    bool       IsTraceOn  () const { return m_bTrace; }
    wostream & TraceStream()       { return wcout; }

    bool                       m_bTrace    { true };
    CommandStack             * m_pCmdStack { nullptr };
    NNetModelCommands        * m_pCommands { nullptr };
    NNetModelReaderInterface * m_pNMRI     { nullptr };
    NNetModelWriterInterface * m_pNMWI     { nullptr };
};