// HistCacheItem.h : 
//

#pragma once

#include "generationCmd.h"
#include "HistoryGeneration.h"
#include "ModelData.h"

class HistCacheItem
{
public:

    HistCacheItem( ModelData * const pModelData ) :
		m_pModelData( pModelData ),
        m_genCmd( ),
        m_genHistCounter( 0L )
    { 
		int s = sizeof(m_genCmd);
		int x = 786;
	}

    virtual ~HistCacheItem( ) { };

	static HistCacheItem * CreateItem( ModelFactory const * const pModelFactory )
	{
		ModelData * pModelDataNew = pModelFactory->CreateModelData( );
		return new HistCacheItem( pModelDataNew );
	}

	virtual void CopyCacheItem( HistCacheItem const * const pSrc )
    {
//        std::wcout << __FUNCTION__ << L" Gen" << pSrc->m_genHistCounter << L" ->" << m_genHistCounter << endl;
        m_pModelData->CopyModelData( pSrc->m_pModelData );
        m_genHistCounter = pSrc->m_genHistCounter;
        m_genCmd         = pSrc->m_genCmd;
    }

    ModelData       * GetModelData( )            { return m_pModelData; }
    ModelData const * GetModelDataC( )     const { return m_pModelData; }
    HIST_GENERATION   GetHistGenCounter( ) const { return m_genHistCounter; }
    GenerationCmd     GetGenCmd( )         const { return m_genCmd; }

    void ZeroHistGenCounter( ) { m_genHistCounter = 0L; }
    void IncHistGenCounter ( ) { ++ m_genHistCounter; }

    void InitializeGenCmd( ) { m_genCmd.InitializeCmd( ); }
    void SetGenerationCommand( GenerationCmd cmd ) { m_genCmd = cmd; }

private:
    ModelData * const m_pModelData;
    HIST_GENERATION   m_genHistCounter;
    GenerationCmd     m_genCmd;
};
