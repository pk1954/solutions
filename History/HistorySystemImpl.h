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

    virtual void InitHistorySystem
    ( 
        HistSlotNr          const, 
        HIST_GENERATION     const, 
        ModelData         * const, 
        ModelFactory      * const,
		ObserverInterface * const,
		GenerationCmd       const
    );

	virtual bool			  AddHistorySlot( )        const;

    virtual HistSlotNr        GetNrOfUsedHistCacheSlots( ) const { return m_pHistoryCache->GetNrOfUsedHistCacheSlots( );    }
    virtual HistSlotNr        GetNrOfHistCacheSlots( )     const { return m_pHistoryCache->GetNrOfHistCacheSlots( );        }
    virtual HIST_GENERATION   GetNrOfGenerations( )        const { return m_pHistoryCache->GetYoungestGeneration( ) + 1;    }
    virtual HIST_GENERATION   GetYoungestGeneration( )     const { return m_pHistoryCache->GetYoungestGeneration( );        }
	virtual HIST_GENERATION   GetCurrentGeneration( )      const { return m_pHistCacheItemWork->GetHistGenCounter( );       }
    virtual BYTES             GetSlotSize( )               const { return m_pHistCacheItemWork->GetItemSize( );             }
	virtual bool              IsInHistoryMode( )           const { return GetCurrentGeneration() < GetYoungestGeneration(); };

	virtual void              ShutDownHistCacheSlot( HistSlotNr const i ) { m_pHistoryCache->ShutDownHistCacheSlot( i ); }

	virtual void              CreateAppCommand( GenerationCmd );
	virtual void              ClearHistory    ( HIST_GENERATION const );
    virtual void              ApproachHistGen ( HIST_GENERATION const );
	virtual tGenCmd           GetGenerationCmd( HIST_GENERATION const );

	virtual HistoryIterator * CreateHistoryIterator( ) const;

    virtual HIST_GENERATION   FindGenerationWithProperty( GenerationProperty const &, bool const ) const;

private:

    GenCmdList      m_GenCmdList;
    HistoryCache  * m_pHistoryCache;
    HistCacheItem * m_pHistCacheItemWork;      // The reference item, where history system gets and restores data 

    void                  save2History( );
    void                  step2NextGeneration( GenerationCmd );
	HistCacheItem const * getCachedItem      ( GenerationCmd );
    void                  checkHistoryStructure( );
};

