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
        Sound            * const,
        NNetCommandStack * const
    );
    void SetModelInterface(NNetModelWriterInterface* const);

    void AddModel();
    //void Connect(NobId const, NobId const);

private:

    wostream& TraceStream() { return wcout; }

    bool                       m_bTrace    { true };
    CommandStack             * m_pCmdStack { nullptr };
    NNetModelWriterInterface * m_pNMWI     { nullptr };
    NNetModelIO              * m_pModelIO  { nullptr };
    Sound                    * m_pSound    { nullptr };
};