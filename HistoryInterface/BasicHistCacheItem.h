// BasicHistCacheItem.h : 
//

#pragma once

#include "generationCmd.h"
#include "HistoryGeneration.h"

class BasicHistCacheItem
{
public:

    BasicHistCacheItem( ) :
        m_genCmd( ),
        m_genHistCounter( 0L )
    { }

    virtual ~BasicHistCacheItem( ) { };

    virtual BasicHistCacheItem * CreateItem   ( )                                  = 0;
    virtual void                 CopyCacheItem( BasicHistCacheItem const * const ) = 0;

    HIST_GENERATION GetHistGenCounter( ) const { return m_genHistCounter; }
    GenerationCmd   GetGenCmd( )         const { return m_genCmd; }

    void ZeroHistGenCounter( ) { m_genHistCounter = 0L; }
    void IncHistGenCounter( ) { ++m_genHistCounter; }

    void InitializeGenCmd( ) { m_genCmd.InitializeCmd( ); }
    void SetGenerationCommand( GenerationCmd cmd ) { m_genCmd = cmd; }

private:
    HIST_GENERATION m_genHistCounter;
    GenerationCmd   m_genCmd;
};
