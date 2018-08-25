// GenCmdList.h
//

#pragma once

#include <vector>
#include "HistoryGeneration.h"
#include "GenerationCmd.h"

class GenCmdList
{
public:
    void Resize( HIST_GENERATION const genMaxNrOfGens )
    {
        m_generationCommands.resize( genMaxNrOfGens.GetLong( ) );
    }

    GenerationCmd & operator[] ( HIST_GENERATION const gen )
    { 
        return m_generationCommands.at( gen.GetLong( ) );
    }
    
    void SetGenerationCmd( HIST_GENERATION const gen, GenerationCmd const cmd )
    {
        m_generationCommands[ gen.GetLong( ) ] = cmd;
    }

    void SetCachedGeneration( HIST_GENERATION const gen, short const sSlotNr )
    {
        m_generationCommands[ gen.GetLong( ) ] = GenerationCmd::CachedCmd( sSlotNr );
    }

    void ResetGenerationCmd( HIST_GENERATION const gen )
    {
        m_generationCommands[ gen.GetLong( ) ].InitializeCmd( );
    }

    bool IsCachedGeneration( HIST_GENERATION const gen ) const
    {
        return m_generationCommands[ gen.GetLong( ) ].IsCachedGeneration( );
    }

    long GetCmdListSize( ) const
    {
        return static_cast<long>( m_generationCommands.size( ) );
    }

private:

    std::vector< GenerationCmd > m_generationCommands;     // m_generationCommands[n] describes the operation to transform generation n-1 to generation n 
};
