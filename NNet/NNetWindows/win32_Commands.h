// win32_Commands.h
//
// NNetWindows
//
// Handle commands with animations

#pragma once

#include <string>
#include "ConnAnimationCommand.h"

class NNetCommand;
class CommandStack;
class NobIdList;
class PixelCoordsFp;
class NNetModelCommands;
class NNetModelReaderInterface;
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
        NNetModelReaderInterface * const,
        NNetModelWriterInterface * const
   );

    void Connect        (NobId const, NobId const);
    bool MakeIoConnector();
    void AnimateArrows  (MicroMeter &, MicroMeter const);
    void AnimateCoord   (PixelCoordsFp &, PixelCoordsFp const);

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