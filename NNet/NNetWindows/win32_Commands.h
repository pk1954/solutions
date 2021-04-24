// win32_Commands.h
//
// NNetWindows

#pragma once

#include <string>

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

    void AlignDirection (RootWindow * const, function<void()> const &);
    void AlignShapes    (RootWindow * const, function<void()> const &);
    void PackShapes     (RootWindow * const, function<void()> const &);
    void CreateConnector(RootWindow * const);
    
    unique_ptr<ShapePtrList<ConnNeuron>> CreateShapeList();

private:

    bool       IsTraceOn  () const { return m_bTrace; }
    wostream & TraceStream()       { return wcout; }

    bool                       m_bTrace    { true };
    CommandStack             * m_pCmdStack { nullptr };
    NNetModelCommands        * m_pCommands { nullptr };
    NNetModelWriterInterface * m_pNMWI     { nullptr };
};