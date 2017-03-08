// history.cpp
//

#include "stdafx.h"
#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif
#include "assert.h"
#include "history.h"

class HIST_SLOT
{
public:
	int m_lGeneration;
	HIST_SLOT *m_pPrev;
	HIST_SLOT *m_pNext;
	char *m_pSlot;
};

class HISTORY_IMPL: public History
{
public:
#ifdef _DEBUG
	virtual void Dump( );
#endif
	virtual void Add( );
	virtual void Discard( );
	virtual bool Forward( );
	virtual bool Backward( );
	virtual long GetGeneration( );
	virtual long GetMemorySize();
	virtual void Reset();

private:

	HIST_SLOT *m_aHistSlot;		   // is tail of list
	HIST_SLOT *m_pHead;            // slot with youngest generation
	HIST_SLOT *m_pCurrent;		   // used in history mode	
	HIST_SLOT *m_pUnused;          // first unused slot
	HIST_SLOT *m_pStartSearching;  // for optimization
	char *m_pMaster;
	long  m_lSlotSize;
	long  m_lNrOfSlots;
	long  m_lGenCount;

	HIST_SLOT * findSlot4Reuse( HIST_SLOT * );

	friend History * History::GetInstance( char *, long, long );
};

History * History::GetInstance( char *pMaster, long lSlotSize, long lNrOfSlots )
{
	HISTORY_IMPL *pHist = new HISTORY_IMPL;
	pHist->m_pMaster    = pMaster;
	pHist->m_lSlotSize  = lSlotSize;
	pHist->m_lNrOfSlots = lNrOfSlots;
	pHist->m_aHistSlot = new  HIST_SLOT[lNrOfSlots];

	for ( HIST_SLOT *pRun = pHist->m_aHistSlot; pRun < pHist->m_aHistSlot + lNrOfSlots; pRun++ )
	{
		pRun->m_pSlot = new char[lSlotSize];
		assert( pRun->m_pSlot != (char *)0 );
	}

	pHist->Reset( );

	return (History *)pHist;
}

void HISTORY_IMPL::Reset( )
{
	 HIST_SLOT *pRun;

	m_lGenCount = 0;
	m_pHead = (HIST_SLOT *)0;
	m_pCurrent = (HIST_SLOT *)0;
	m_pStartSearching = (HIST_SLOT *)0;
	m_pUnused = m_aHistSlot;

	pRun = m_aHistSlot;
	pRun->m_pPrev = (HIST_SLOT *)0;
	pRun->m_pNext = pRun + 1;

	for ( pRun = m_aHistSlot + 1; pRun < m_aHistSlot + m_lNrOfSlots - 1; pRun++ )
	{
		pRun->m_pPrev = pRun - 1;
		pRun->m_pNext = pRun + 1;
	}

	pRun = m_aHistSlot + m_lNrOfSlots - 1;
	pRun->m_pPrev = pRun - 1;
	pRun->m_pNext = (HIST_SLOT *)0;
}
/*
bool HISTORY_IMPL::Back2Oldest( )
{
	if (m_lGenCount == 0)
		return false;
	else
	{
		m_pCurrent = m_aHistSlot;
		return true;
	}
}
*/
bool HISTORY_IMPL::Backward( )
{
	if (m_pCurrent == m_aHistSlot)
		return false;	
	
	m_pCurrent = (m_pCurrent == (HIST_SLOT *)0) ? m_pHead : m_pCurrent->m_pPrev;

	memcpy( m_pMaster, m_pCurrent->m_pSlot, m_lSlotSize );
	return true;
}

bool HISTORY_IMPL::Forward( )
{
	bool bHistoryMode = (m_pCurrent != (HIST_SLOT *)0) && (m_pCurrent != m_pHead);

	if ( bHistoryMode )
	{
		m_pCurrent = m_pCurrent->m_pNext;
		memcpy( m_pMaster, m_pCurrent->m_pSlot, m_lSlotSize );
	}
	else
	{
		Add();
		m_pCurrent = (HIST_SLOT *)0;
		memcpy( m_pHead->m_pSlot, m_pMaster, m_lSlotSize );
	}

	return bHistoryMode;
}

long HISTORY_IMPL::GetGeneration( )
{
	if (m_pCurrent != (HIST_SLOT *)0) 
		return m_pCurrent->m_lGeneration;
	else if (m_pHead != (HIST_SLOT *)0) 
		return m_pHead->m_lGeneration;
	else
		return 0;
}

long HISTORY_IMPL::GetMemorySize()
{
	return m_lNrOfSlots;
}

HIST_SLOT * HISTORY_IMPL::findSlot4Reuse( HIST_SLOT * pCandidate )
{
	if ( m_pStartSearching == (HIST_SLOT *)0 )  // happens only once
	{
		pCandidate = pCandidate->m_pPrev->m_pPrev->m_pPrev;  
	}
	else // Go 2 steps forward (if possible) to ensure that pCandidate->m_pPrev->m_pPrev exists
	{
		if ( m_pStartSearching->m_pNext != (HIST_SLOT *)0 )
		{
			m_pStartSearching = m_pStartSearching->m_pNext;

			if ( m_pStartSearching->m_pNext != (HIST_SLOT *)0 )
				m_pStartSearching = m_pStartSearching->m_pNext;
		}

		pCandidate = m_pStartSearching->m_pPrev;
	}

	while ( pCandidate->m_pPrev->m_pPrev != (HIST_SLOT *)0 )
	{
		long thisDiff = pCandidate->m_pNext->m_lGeneration - pCandidate->m_pPrev->m_lGeneration;
		long lastDiff = pCandidate->m_pPrev->m_lGeneration - pCandidate->m_pPrev->m_pPrev->m_lGeneration;

		if ( lastDiff == 1 )
			m_pStartSearching = pCandidate;

		if ( thisDiff + 1 <= lastDiff )
			break;

		pCandidate = pCandidate->m_pPrev;
	} 

	return pCandidate;
}

void HISTORY_IMPL::Add( )
{
	HIST_SLOT *pPrev = m_pHead;

	if ( m_pUnused != (HIST_SLOT *)0 )          // Unused slots available
	{
		m_pHead = m_pUnused;
		m_pUnused = m_pUnused->m_pNext;
	}
	else                                       // No unused slots. We have to reuse slots
	{
		m_pHead = findSlot4Reuse( m_pHead );
		m_pHead->m_pNext->m_pPrev = m_pHead->m_pPrev;
		m_pHead->m_pPrev->m_pNext = m_pHead->m_pNext;
	}

	if ( pPrev != (HIST_SLOT *)0 )
		pPrev->m_pNext = m_pHead;
	m_pHead->m_pNext = (HIST_SLOT *)0;
	m_pHead->m_pPrev = pPrev;
	m_pHead->m_lGeneration = ++m_lGenCount;
}

void HISTORY_IMPL::Discard( )
{
	if (m_pCurrent == (HIST_SLOT *)0)  // nothing to do
		return;

	if (m_pCurrent == m_pHead)  // we are back to present
		return;

	m_pHead = m_pCurrent;
	m_pUnused = m_pHead->m_pNext;
	m_pHead->m_pNext = (HIST_SLOT *)0;
	m_lGenCount = m_pHead->m_lGeneration;
}

#ifdef _DEBUG
void HISTORY_IMPL::Dump( )
{
	cout << "Mem: ";

	for ( HIST_SLOT *pRun = m_aHistSlot; pRun != (HIST_SLOT *)0; pRun = pRun->m_pNext )
	{
		cout.width(6);
		cout << pRun->m_lGeneration;
	}

	cout << endl;
/*
	cout << "Dif:     ";
	
	if (hHist->Back2Oldest() ) do
	{
		int iGen = hHist->GetGeneration();
		if ( !hHist->Forward() )
			break;
		cout.width(6);
		cout << hHist->GetGeneration() - iGen;
	} while (true);

	cout << endl;
*/
}
#endif

