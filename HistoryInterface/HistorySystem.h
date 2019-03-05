// HistorySystem.h
//

#pragma once

#include "GenerationCmd.h"
#include "HistoryGeneration.h"

class ObserverInterface;
class ObserverInterface;
class HistoryIterator;
class ModelFactory;
class ModelData;

class GenerationProperty  // used when searching for generation with certain property
{
public:
    virtual bool operator() ( ModelData const * ) const = 0;
};

class HistoryBufferException : public std::exception { };

class HistorySystem
{
public:
	static HistorySystem * CreateHistorySystem( );

    virtual ~HistorySystem( ) { };

    virtual void InitHistorySystem
	( 
		HistSlotNr          const, 
		HIST_GENERATION     const, 
		ModelData         * const, 
		ModelFactory      * const, 
		ObserverInterface * const,
		GenerationCmd       const
	) = 0;

    virtual int               GetSlotSize( )               const = 0;
    virtual HistSlotNr        GetNrOfUsedHistCacheSlots( ) const = 0;
    virtual HistSlotNr        GetNrOfHistCacheSlots( )     const = 0;
    virtual HIST_GENERATION   GetNrOfGenerations( )        const = 0;
    virtual HIST_GENERATION   GetYoungestGeneration( )     const = 0;
    virtual HIST_GENERATION   GetCurrentGeneration( )      const = 0;
    virtual bool              IsInHistoryMode( )           const = 0;
    virtual HistoryIterator * CreateHistoryIterator( )     const = 0;
    virtual bool              AddHistorySlot( )            const = 0;

	virtual void    ShutDownHistCacheSlot( HistSlotNr const ) = 0;
    virtual void    CreateAppCommand( GenerationCmd   const ) = 0;
	virtual void    ClearHistory    ( HIST_GENERATION const ) = 0;
    virtual void    ApproachHistGen ( HIST_GENERATION const ) = 0;
	virtual tGenCmd GetGenerationCmd( HIST_GENERATION const ) = 0;

    virtual HIST_GENERATION FindGenerationWithProperty( GenerationProperty const &, bool const ) const = 0;
};
