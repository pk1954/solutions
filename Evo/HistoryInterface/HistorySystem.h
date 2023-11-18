// HistorySystem.h
//

module;

import Observable;
import GenerationCmd;
import HistoryGeneration;

export module HistorySystem;

class ObserverInterface;
class HistoryIterator;
class HistoryCache;
class ModelFactory;
class ModelData;

class GenerationProperty  // used when searching for generation with certain property
{
public:
    virtual bool operator() ( ModelData const * ) const = 0;
};

class HistoryBufferException : public std::exception { };

class HistorySystem : public Observable
{
public:
	static HistorySystem * CreateHistorySystem( );

    virtual ~HistorySystem( ) { };

    virtual ModelData * StartHistorySystem
	( 
		HIST_GENERATION    const, 
		long               const, 
		unsigned long long const, 
		ModelFactory     * const
	) = 0;

	virtual void StopHistorySystem( ) = 0;

    virtual BYTES             GetSlotSize( )               const = 0;
    virtual HistSlotNr        GetNrOfUsedHistCacheSlots( ) const = 0;
    virtual HistSlotNr        GetNrOfHistCacheSlots( )     const = 0;
    virtual HIST_GENERATION   GetNrOfGenerations( )        const = 0;
    virtual HIST_GENERATION   GetYoungestGeneration( )     const = 0;
    virtual HIST_GENERATION   GetCurrentGeneration( )      const = 0;
    virtual bool              IsInHistoryMode( )           const = 0;
    virtual HistoryIterator * CreateHistoryIterator( )     const = 0;
    virtual bool              AddHistorySlot( )                  = 0;

    virtual ModelData const * CreateAppCommand( GenerationCmd   const ) = 0;
	virtual ModelData const * ApproachHistGen ( HIST_GENERATION const ) = 0;
	virtual GenerationCmd     GetGenerationCmd( HIST_GENERATION const ) = 0;
	virtual void              ClearHistory    ( HIST_GENERATION const ) = 0;
	virtual void              ClearAllHistory ( )                       = 0;

	virtual HistoryCache const * GetHistoryCache( ) const = 0;

	virtual ModelData const * GetModelData( HIST_GENERATION const ) = 0;

	virtual void              ShutDownHistCache( ) = 0;

	virtual HIST_GENERATION   FindGenerationWithProperty( GenerationProperty const &, bool const ) const = 0;

    static  BYTES GetSlotWrapperSize( );
};
