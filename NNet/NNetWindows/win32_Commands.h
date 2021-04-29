// win32_Commands.h
//
// NNetWindows

#pragma once

#include <string>
#include "ConnAnimationCommand.h"

class Command;
class CommandStack;
class NNetModelCommands;
class NNetModelWriterInterface;
class RootWindow;

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
        NNetModelWriterInterface * const
    );

    void Update(ConnAnimationCommand * const);
    //void AlignDirection  (RootWindow *, int const, bool const);
    //void AlignPositions  (RootWindow *, int const, bool const);
    //void PackShapes      (RootWindow *, int const, bool const);
    void AlignShapes     (RootWindow *);
    void MakeConnector   (RootWindow *);
    void ConnectConnector(RootWindow *);
//    void CreateConnector();
    
    CommandStack & GetCommandStack() { return * m_pCmdStack; }
    NNetModelWriterInterface & GetNMWI() { return * m_pNMWI; }
    unique_ptr<ShapePtrList<ConnNeuron>> CreateShapeList();

private:

    bool       IsTraceOn  () const { return m_bTrace; }
    wostream & TraceStream()       { return wcout; }

    bool                       m_bTrace    { true };
    CommandStack             * m_pCmdStack { nullptr };
    NNetModelCommands        * m_pCommands { nullptr };
    NNetModelWriterInterface * m_pNMWI     { nullptr };
};