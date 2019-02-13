// strategy.cpp : 
//

#include "stdafx.h"
#include "assert.h"
#include "random.h"
#include "strategy.h"

MEM_INDEX    StrategyData::m_uiMaxPartnerMemory                 = MEM_INDEX(0);
unsigned int StrategyData::m_uiNrInteractionsWithKnownCulprit   = 0;
unsigned int StrategyData::m_uiNrInteractionsWithUnknownCulprit = 0;

StrategyData::StrategyData( )
{
    m_memSize = MEM_INDEX(0);
    m_memUsed = MEM_INDEX(0);
    for	( auto & i : m_aIdBadGuys )
        i.Set2Null();
}

IndividualId StrategyData::GetMemEntry( MEM_INDEX const index ) const 
{
    return ( index < m_memUsed )
		   ? getBadGuyId( index )
		   : IndividualId::NULL_VAL();  // Can happen at race conditions
}	

MEM_INDEX StrategyData::FindInList( IndividualId const & idPartner ) 
{
    for	( MEM_INDEX index = MEM_INDEX(0); index < m_memUsed; ++index )
    {
        if ( getBadGuyId( index ) == idPartner )
        {
            ++m_uiNrInteractionsWithKnownCulprit;
            return index;
        }
    }

    return MEM_INDEX::NULL_VAL();     
}

void StrategyData::RemoveFromList( MEM_INDEX const index )
{
    assert( m_memUsed > index );
 
    MEM_INDEX const indexStop = --m_memUsed;
    for	( MEM_INDEX iRun = index; iRun < indexStop; ++iRun )
        getBadGuyId(iRun) = getBadGuyId(iRun + MEM_INDEX(1));

    getBadGuyId(m_memUsed).Set2Null( );
}

void StrategyData::AddToList( IndividualId const & partnerId )
{
    ++m_uiNrInteractionsWithUnknownCulprit;           

    if ( m_memUsed == m_memSize )          // list is full. Remove oldest entry
        RemoveFromList( MEM_INDEX(0) );

    getBadGuyId(m_memUsed++) = partnerId;   // add id to list of bad guys

    if ( m_memUsed > m_uiMaxPartnerMemory )  // statistics of memory usage
        m_uiMaxPartnerMemory = m_memUsed;
}

void StrategyData::SetMemorySize( MEM_INDEX const newSize )
{
    assert( newSize <= MEM_INDEX(IMEMSIZE_MAX) );

    m_memSize = newSize;
    m_memUsed = MEM_INDEX(0);
}

bool Tit4Tat::InteractWith( StrategyData &data, IndividualId const idPartner ) const
{
    bool const bNotorious = data.FindInList( idPartner ).IsNotNull();
    return ! bNotorious;                                    // If he is not in the list of bad guys, be friendly
}

void Tit4Tat::Remember( StrategyData &data, IndividualId const idPartner, bool const bPartnerReaction ) const
{
    MEM_INDEX const index = data.FindInList( idPartner );

    if ( index.IsNotNull() )               // He is on the list of bad guys
    {
        if ( bPartnerReaction )            // He turned into a good guy. Let him off the hook
        {
            MEM_INDEX const memIndex = index;
            assert( data.GetMemUsed( ) > memIndex );
            data.RemoveFromList( memIndex );
        }
        else                               // He is persistently bad. Remains on the list.
        {
        }
    }
    else 	                               // We don't know him
    {
        if ( bPartnerReaction )            // He seems to be a good guy
        {
        }
        else                               // He doesn't behave well. Put him on the list
        {
            data.AddToList( idPartner );   
        }
    }
};

