// HistorySystem.h
//

#pragma once

#include "HistoryGeneration.h"
#include "generationCmd.h"

#ifndef NDEBUG
#define CHECK_HISTORY_STRUCTURE checkHistoryStructure( )
#else
#define CHECK_HISTORY_STRUCTURE
#endif

class HistoryCache;
class DisplayFunctor;
class NextGenFunctor;
class HistoryIterator;
class BasicHistCacheItem;
class GenCmdList;

class GenerationProperty
{
public:
    virtual bool operator() ( BasicHistCacheItem const * ) const = 0;
};

class HistorySystem
{
public:
    HistorySystem( );
    ~HistorySystem( );

    void InitHistoryComputation
    ( 
        short const, 
        HIST_GENERATION const, 
        NextGenFunctor const * const, 
        BasicHistCacheItem *
    );

    void SetAskHistoryCutFunctor( DisplayFunctor const * const f ) { m_pAskHistoryCutFunctor   = f; }

    int               GetNrOfHistCacheSlots( ) const;
    HIST_GENERATION   GetNrOfGenerations( )    const;
    HIST_GENERATION   GetYoungestGeneration( ) const;
    HIST_GENERATION   GetCurrentGeneration( )  const;
    bool              IsInHistoryMode( )       const;
    HistoryIterator * CreateHistoryIterator( ) const;

    bool              AddHistorySlot( );
    void              ShutDownHistCacheSlot( short const );

    bool              CreateNewGeneration( GenerationCmd const );
    void              ApproachHistGen    ( HIST_GENERATION const );

    HIST_GENERATION   FindFirstGenerationWithProperty( GenerationProperty const & ) const;
    HIST_GENERATION   FindLastGenerationWithProperty ( GenerationProperty const & ) const;

private:

    GenCmdList           * m_pGenCmdList;
    HistoryCache         * m_pHistoryCache;
    BasicHistCacheItem   * m_pHistCacheItemWork;      // The reference item, where history system gets and restores  
    DisplayFunctor const * m_pAskHistoryCutFunctor;   // GUI callback for asking user if history should be cut off 
    NextGenFunctor const * m_pNextGenerationFunctor;  // application callback for advancing to the next generation

    void save2History( BasicHistCacheItem const & );
    void step2NextGeneration( GenerationCmd );
    void checkHistoryStructure( );
};
