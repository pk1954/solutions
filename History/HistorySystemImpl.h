// HistorySystemImpl.h
//

#pragma once

#include "HistoryGeneration.h"
#include "HistorySystem.h"
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
class HistCacheItem;
class GenCmdList;
class ModelFactory;
class ModelData;

class HistorySystemImpl: public HistorySystem
{
public:
    HistorySystemImpl( );
    ~HistorySystemImpl( );

    virtual void InitHistorySystem
    ( 
        short const, 
        HIST_GENERATION const, 
        ModelData     * const, 
        ModelFactory  * const
    );

    virtual int               GetNrOfHistCacheSlots( ) const;
    virtual HIST_GENERATION   GetNrOfGenerations( )    const;
    virtual HIST_GENERATION   GetYoungestGeneration( ) const;
    virtual HIST_GENERATION   GetCurrentGeneration( )  const;
    virtual bool              IsInHistoryMode( )       const;
    virtual HistoryIterator * CreateHistoryIterator( ) const;

    virtual bool              AddHistorySlot( ) const;
    virtual void              ShutDownHistCacheSlot( short const );

    virtual void              CreateAppCommand( unsigned short const, short const );
	virtual void              ClearHistory    ( HIST_GENERATION const );
    virtual void              ApproachHistGen ( HIST_GENERATION const );

    virtual HIST_GENERATION   FindFirstGenerationWithProperty( GenerationProperty const & ) const;
    virtual HIST_GENERATION   FindLastGenerationWithProperty ( GenerationProperty const & ) const;

private:

    GenCmdList           * m_pGenCmdList;
    HistoryCache         * m_pHistoryCache;
    HistCacheItem        * m_pHistCacheItemWork;      // The reference item, where history system gets and restores  
    DisplayFunctor const * m_pAskHistoryCutFunctor;   // GUI callback for asking user if history should be cut off 
    ModelData            * m_pModelDataWork; 
	ModelFactory   const * m_pModelFactory;

	void createNewGen( GenerationCmd );
    void save2History( HistCacheItem const & );
    void step2NextGeneration( GenerationCmd );
    void checkHistoryStructure( );
};

