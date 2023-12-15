// GenCmdList.ixx
//
// History

module;

#include <vector>

export module GenCmdList;

import GenerationCmd;
import HistGeneration;
import HistSlotNr;

export class GenCmdList
{
public:
    void Resize(HistGeneration const genMaxNrOfGens)
    {
        m_generationCommands.resize(genMaxNrOfGens.GetLong());
    }

    GenerationCmd& operator[] (HistGeneration const gen)
    {
        return m_generationCommands.at(gen.GetLong());
    }

    void SetGenerationCmd(HistGeneration const gen, GenerationCmd const cmd)
    {
        m_generationCommands[gen.GetLong()] = cmd;
    }

    void SetCachedGeneration(HistGeneration const gen, HistSlotNr const slotNr)
    {
        m_generationCommands[gen.GetLong()] = GenerationCmd::CachedCmd(slotNr);
    }

    void ResetGenerationCmd(HistGeneration const gen)
    {
        m_generationCommands[gen.GetLong()].InitializeCmd();
    }

    bool IsCachedGeneration(HistGeneration const gen) const
    {
        return m_generationCommands[gen.GetLong()].IsCachedGeneration();
    }

    long GetMaxGeneration() const
    {
        return static_cast<long>(m_generationCommands.size() - 1);
    }

private:

    std::vector< GenerationCmd > m_generationCommands;     // m_generationCommands[n] describes the operation to transform generation n-1 to generation n 
};
