// EvoModelData.h
//

#pragma once

#include "gridPoint.h"
#include "ModelData.h"
#include "EvolutionCore.h"
#include "BasicHistCacheItem.h"
#include "EvoGenerationCmd.h"

class EvoModelData: public BasicHistCacheItem
{
public:
    EvoModelData( ):
        BasicHistCacheItem( ),
        m_pModelData( ModelData::CreateModelData( ) )
    { }

    EvoModelData( ModelData * pModelDataWork ):
        BasicHistCacheItem( ),
        m_pModelData( pModelDataWork )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( )
    {
        if ( m_pModelData != nullptr )
            delete m_pModelData;
    }

    ModelData * GetModelData( ) { return m_pModelData; }

    ModelData const * GetModelDataC( ) const { return m_pModelData; }

    GridPoint FindGridPoint( IndId const & id ) const
    { 
        return m_pModelData->FindGridPoint( id );
    }

// implement abstract interface of BasicHistCacheItem

    virtual EvoModelData * CreateItem( ) 
    { 
        return new EvoModelData( );
    }

    virtual void EvoModelData::CopyCacheItem( BasicHistCacheItem const * const pSrc )
    {
        m_pModelData->CopyModelData( static_cast<EvoModelData const * const>( pSrc )->GetModelDataC( ) );
        * static_cast<BasicHistCacheItem * const>( this ) = * pSrc;
    }

private:
    ModelData * const m_pModelData;
};

