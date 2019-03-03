// strategy.h : 
//

#pragma once
#include <array>
#include "random.h"
#include "EvolutionTypes.h"

class StrategyData
{
public:
    StrategyData( );

    void SetMemorySize( MEM_INDEX const );

    // display information

    IND_ID    GetMemEntry( MEM_INDEX const ) const;	
    MEM_INDEX GetMemSize ( )                 const { return m_memSize; };
    MEM_INDEX GetMemUsed ( )                 const { return m_memUsed; };

    static MEM_INDEX    GetMaxPartnerMemory( )                 { return m_uiMaxPartnerMemory; };
    static unsigned int GetNrInteractionsWithKnownCulprit( )   { return m_uiNrInteractionsWithKnownCulprit; };
    static unsigned int GetNrInteractionsWithUnknownCulprit( ) { return m_uiNrInteractionsWithUnknownCulprit; };

	MEM_INDEX FindInList( IND_ID const & );
    void      AddToList ( IND_ID const & );
    void      RemoveFromList( MEM_INDEX const );

private:
    static MEM_INDEX    m_uiMaxPartnerMemory;
    static unsigned int m_uiNrInteractionsWithKnownCulprit;
    static unsigned int m_uiNrInteractionsWithUnknownCulprit;

    MEM_INDEX m_memUsed;   // number of occopied slots.     
    MEM_INDEX m_memSize;   // number of usable slots.    m_memUsed <= m_memSize <= IMEMSIZE_MAX

    std::array< IND_ID, IMEMSIZE_MAX > m_aIdBadGuys;

	IND_ID getBadGuyId( MEM_INDEX const index ) const
	{
		return m_aIdBadGuys[ index.GetValue() ];
	}
};

class DefectAlways: public Strategy
{
public:
    virtual Id   GetStrategyId( )                                              const { return Id::defect; };
    virtual bool InteractWith( StrategyData &mData, IND_ID const lPartnerId )  const { return false; };
    virtual void Remember    ( StrategyData &mData, IND_ID const, bool const ) const { };
};

class CooperateAlways: public Strategy
{
public:
    virtual Id   GetStrategyId( )                                              const { return Id::cooperate; };
    virtual bool InteractWith( StrategyData &mData, IND_ID const lPartnerId )  const { return true; };
    virtual void Remember    ( StrategyData &mData, IND_ID const, bool const ) const { };
};

class Tit4Tat: public Strategy
{
public:
    virtual Id   GetStrategyId( )                                         const { return Id::tit4tat; };
    virtual bool InteractWith( StrategyData &, IND_ID const )             const;
    virtual void Remember    ( StrategyData &, IND_ID const, bool const ) const;
};

class EmptyStrategy: public Strategy
{
public:
    virtual Id   GetStrategyId( )                                         const { return Id::empty; };
	virtual bool InteractWith( StrategyData &, IND_ID const )             const { return false; };
	virtual void Remember    ( StrategyData &, IND_ID const, bool const ) const {};
};
