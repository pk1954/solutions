// strategy.cpp : 
//

#include "stdafx.h"
#include "assert.h"
#include "random.h"
#include "strategy.h"

MEM_INDEX    StrategyData::m_uiMaxPartnerMemory                 = 0;
unsigned int StrategyData::m_uiNrInteractionsWithKnownCulprit   = 0;
unsigned int StrategyData::m_uiNrInteractionsWithUnknownCulprit = 0;

StrategyData::StrategyData( )
{
    m_memSize = 0;
    m_memUsed = 0;
    for	( auto & i : m_aIdBadGuys )
        i.ResetIndId();
}

IndId StrategyData::GetMemEntry( MEM_INDEX const index ) const 
{
    return ( index < m_memUsed )
		   ? m_aIdBadGuys[index]
		   : IndId::NO_INDIVIDUAL;  // Can happen at race conditions
}	

int StrategyData::findInList( IndId const & idPartner ) 
{
    for	( MEM_INDEX index = 0; index < m_memUsed; ++index )
    {
        if ( m_aIdBadGuys[index] == idPartner )
        {
            ++m_uiNrInteractionsWithKnownCulprit;
            return static_cast<int>(index);
        }
    }

    return -1;     
}

void StrategyData::removeFromList( MEM_INDEX const index )
{
    assert( m_memUsed > index );
 
    MEM_INDEX const indexStop = --m_memUsed;
    for	( MEM_INDEX iRun = index; iRun < indexStop; ++iRun )
        m_aIdBadGuys[iRun] = m_aIdBadGuys[iRun+1];

    m_aIdBadGuys[m_memUsed].ResetIndId( );
}

void StrategyData::addToList( IndId const & partnerId )
{
    ++m_uiNrInteractionsWithUnknownCulprit;           

    if ( m_memUsed == m_memSize )          // list is full. Remove oldest entry
        removeFromList( 0 );

    m_aIdBadGuys[m_memUsed++] = partnerId;   // add id to list of bad guys

    if ( m_memUsed > m_uiMaxPartnerMemory )  // statistics of memory usage
        m_uiMaxPartnerMemory = m_memUsed;
}

void StrategyData::SetMemorySize( MEM_INDEX const newSize )
{
    assert( newSize <= IMEMSIZE_MAX );

    m_memSize = newSize;
    m_memUsed = 0;
}

bool Tit4Tat::InteractWith( StrategyData &data, IndId const idPartner ) const
{
    bool const bKnown = data.findInList( idPartner ) >= 0;
    return (!bKnown);                                    // If he is not in the list of bad guys, be friendly
}

void Tit4Tat::Remember( StrategyData &data, IndId const idPartner, bool const bPartnerReaction ) const
{
    int const index = data.findInList( idPartner );

    if ( index >= 0 )                      // He is on the list of bad guys
    {
        if ( bPartnerReaction )            // He turned into a good guy. Let him off the hook
        {
            MEM_INDEX const memIndex = static_cast<MEM_INDEX>(index);
            assert( data.GetMemUsed( ) > memIndex );
            data.removeFromList( memIndex );
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
            data.addToList( idPartner );   
        }
    }
};

