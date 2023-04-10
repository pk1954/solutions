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
import NNetCommand;
import NNetCommandStack;
import NNetModel;

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

    void UndoCommand();
    void RedoCommand();

    void AddModel();
    void AnimateArrows(MicroMeter&, MicroMeter const);
    void Connect(NobId const, NobId const);
    void StartStimulus();

    SensorId SetHighlightedSensor(MicroMeterPnt const&);

private:

    wostream& TraceStream() { return wcout; }

    bool                       m_bTrace    { true };
    CommandStack             * m_pCmdStack { nullptr };
    NNetModelWriterInterface * m_pNMWI     { nullptr };
    NNetModelIO              * m_pModelIO  { nullptr };
    Sound                    * m_pSound    { nullptr };
};