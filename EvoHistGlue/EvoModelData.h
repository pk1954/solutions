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
//        if ( m_pModelData != nullptr )
//            delete m_pModelData;
    }

//    ModelData * GetModelData( ) { return m_pModelData; }

//    ModelData const * GetModelDataC( ) const { return m_pModelData; }

    GridPoint FindGridPoint( IndId const & id ) const
    { 
        return GetModelDataC( )->FindGridPoint( id );
    }

// implement abstract interface of BasicHistCacheItem

    virtual EvoModelData * CreateItem( ) 
    { 
        return new EvoModelData( );
    }

    virtual void CopyModelData( BasicHistCacheItem const * const pSrc )
    {
        GetModelData( )->CopyModelData( static_cast<EvoModelData const * const>( pSrc )->GetModelDataC( ) );
    }

private:
    ModelData * const m_pModelData;
};

