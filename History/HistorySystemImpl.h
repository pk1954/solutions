// HistorySystemImpl.h
//

#pragma once

#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "historyCache.h"
#include "genCmdList.h"
#include "GenerationCmd.h"

#ifndef NDEBUG
#define CHECK_HISTORY_STRUCTURE checkHistoryStructure( )
#else
#define CHECK_HISTORY_STRUCTURE
#endif

class ObserverInterface;
class ObserverInterface;
class NextGenFunctor;
class HistoryIterator;
class HistCacheItem;
class ModelFactory;
class ModelData;

class HistorySystemImpl: public HistorySystem
{
public:
    HistorySystemImpl( );
    ~HistorySystemImpl( );

    virtual ModelData * StartHistorySystem
    ( 
		HIST_GENERATION     const, 
		long                const, 
		unsigned long long  const, 
		ModelFactory      * const,
		ObserverInterface * const,
		GenerationCmd       const
    );

	virtual void StopHistorySystem( );

	virtual bool			  AddHistorySlot( )        const;

    virtual HistSlotNr        GetNrOfUsedHistCacheSlots( ) const { return m_pHistoryCache->GetNrOfUsedHistCacheSlots( );    }
    virtual HistSlotNr        GetNrOfHistCacheSlots( )     const { return m_pHistoryCache->GetNrOfHistCacheSlots( );        }
    virtual HIST_GENERATION   GetNrOfGenerations( )        const { return m_pHistoryCache->GetYoungestGeneration( ) + 1;    }
    virtual HIST_GENERATION   GetYoungestGeneration( )     const { return m_pHistoryCache->GetYoungestGeneration( );        }
	virtual HIST_GENERATION   GetCurrentGeneration( )      const { return m_pHistCacheItemWork->GetHistGenCounter( );       }
    virtual BYTES             GetSlotSize( )               const { return m_pHistCacheItemWork->GetItemSize( );             }
	virtual bool              IsInHistoryMode( )           const { return GetCurrentGeneration() < GetYoungestGeneration(); };

	virtual void              ShutDownHistCache(  ) { m_pHistoryCache->ShutDownHistCache( ); }

	virtual ModelData const * CreateAppCommand( GenerationCmd );
	virtual void              ClearHistory    ( HIST_GENERATION const );
    virtual ModelData const * ApproachHistGen ( HIST_GENERATION const );
	virtual tGenCmd           GetGenerationCmd( HIST_GENERATION const );
	virtual ModelData const * GetModelData    ( HIST_GENERATION const );

	virtual HistoryIterator * CreateHistoryIterator( ) const;

    virtual HIST_GENERATION   FindGenerationWithProperty( GenerationProperty const &, bool const ) const;

private:

    GenCmdList      m_GenCmdList;
    HistoryCache  * m_pHistoryCache;
    HistCacheItem * m_pHistCacheItemWork;      // The reference item, where history system gets and restores data 

	ModelData  const * save2History( );
    void               step2NextGeneration( GenerationCmd );
    void               checkHistoryStructure( );
};

