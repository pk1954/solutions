// EvoModelData.h
//

#pragma once

#include "gridPoint.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "HistCacheItem.h"
#include "EvoGenerationCmd.h"

class EvoModelData: public ModelData
{
public:
/*
    EvoModelData( ):
        HistCacheItem( ),
        m_pModelData( EvolutionModelData::CreateModelData( ) )
    { }
*/
/*
    EvoModelData( EvolutionModelData * pModelDataWork ):
        HistCacheItem( ),
        m_pModelData( pModelDataWork )
    { }
*/
    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( )
    {
//        if ( m_pModelData != nullptr )
//            delete m_pModelData;
    }

//    EvolutionModelData * GetModelData( ) { return m_pModelData; }

//    EvolutionModelData const * GetModelDataC( ) const { return m_pModelData; }
/*
    GridPoint FindGridPoint( IndId const & id ) const
    { 
        return GetModelDataC( )->FindGridPoint( id );
    }
*/
// implement abstract interface of HistCacheItem
/*
    virtual EvoModelData * CreateItem( ) 
    { 
        return EvolutionModelData::CreateModelData( );
    }
*/
    virtual void CopyModelData( ModelData const * const src  )
    {
        * this = * static_cast< EvoModelData const * const >( src );
    }
};

