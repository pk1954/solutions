// NNetModelCommands.ixx
//
// Commands

module;

#include <string>
#include <iostream>

export module NNetModelCommands;

import Types;
import Uniform2D;
import SoundInterface;
import Commands;
import NNetModelIO;
import NNetModel;
import NNetCommands;

using std::wcout;
using std::wostream;
using std::unique_ptr;

export class NNetModelCommands
{
public:
    void Initialize
    (
        NNetModelIO      * const,
        NNetCommandStack * const
    );

    void AddModel();

private:

    wostream& TraceStream() { return wcout; }

    bool           m_bTrace    { true };
    CommandStack * m_pCmdStack { nullptr };
    NNetModelIO  * m_pModelIO  { nullptr };
};