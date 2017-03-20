// HistorySystem.h
//

#pragma once

#include "HistoryGeneration.h"

class DisplayFunctor;
class HistoryIterator;
class ModelFactory;
class ModelData;

class GenerationProperty
{
public:
    virtual bool operator() ( ModelData const * ) const = 0;
};

class HistorySystem
{
public:
	static HistorySystem * CreateHistorySystem( );

    virtual ~HistorySystem( ) { };

    virtual void InitHistorySystem( short const, HIST_GENERATION const, ModelData * const, ModelFactory * const ) = 0;

    virtual int               GetNrOfHistCacheSlots( ) const = 0;
    virtual HIST_GENERATION   GetNrOfGenerations( )    const = 0;
    virtual HIST_GENERATION   GetYoungestGeneration( ) const = 0;
    virtual HIST_GENERATION   GetCurrentGeneration( )  const = 0;
    virtual bool              IsInHistoryMode( )       const = 0;
    virtual HistoryIterator * CreateHistoryIterator( ) const = 0;

    virtual bool              AddHistorySlot( )              const = 0;
    virtual void              ShutDownHistCacheSlot( short const ) = 0;

    virtual bool              CreateAppCommand( unsigned short const, short const ) = 0;
	virtual void              ClearHistory    ( HIST_GENERATION const ) = 0;
    virtual void              ApproachHistGen ( HIST_GENERATION const ) = 0;

    virtual void              SetAskHistoryCutFunctor( DisplayFunctor const * const ) = 0;

    virtual HIST_GENERATION   FindFirstGenerationWithProperty( GenerationProperty const & ) const = 0;
    virtual HIST_GENERATION   FindLastGenerationWithProperty ( GenerationProperty const & ) const = 0;
};
