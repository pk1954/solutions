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

    static MEM_INDEX const IMEMSIZE_MAX = 20;

    void SetMemorySize( MEM_INDEX const );

    // display information

    IndId     GetMemEntry( MEM_INDEX const ) const;	
    MEM_INDEX GetMemSize ( )                 const { return m_memSize; };
    MEM_INDEX GetMemUsed ( )                 const { return m_memUsed; };

    static MEM_INDEX    GetMaxPartnerMemory( )                 { return m_uiMaxPartnerMemory; };
    static unsigned int GetNrInteractionsWithKnownCulprit( )   { return m_uiNrInteractionsWithKnownCulprit; };
    static unsigned int GetNrInteractionsWithUnknownCulprit( ) { return m_uiNrInteractionsWithUnknownCulprit; };

    int  findInList( IndId const & );
    void addToList ( IndId const & );
    void removeFromList( MEM_INDEX const );

private:
    static MEM_INDEX    m_uiMaxPartnerMemory;
    static unsigned int m_uiNrInteractionsWithKnownCulprit;
    static unsigned int m_uiNrInteractionsWithUnknownCulprit;

    MEM_INDEX m_memUsed;   // number of occopied slots.     
    MEM_INDEX m_memSize;   // number of usable slots.    m_memUsed <= m_memSize <= IMEMSIZE_MAX
    std::array< IndId, IMEMSIZE_MAX > m_aIdBadGuys;
};

class Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                        const = 0;
    virtual bool        InteractWith( StrategyData &, IndId const )             const = 0;
    virtual void        Remember    ( StrategyData &, IndId const, bool const ) const = 0;
};

class DefectAlways: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                             const { return tStrategyId::defect; };
    virtual bool        InteractWith( StrategyData &mData, IndId const lPartnerId )  const { return false; };
    virtual void        Remember    ( StrategyData &mData, IndId const, bool const ) const { };
};

class CooperateAlways: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                             const { return tStrategyId::cooperate; };
    virtual bool        InteractWith( StrategyData &mData, IndId const lPartnerId )  const { return true; };
    virtual void        Remember    ( StrategyData &mData, IndId const, bool const ) const { };
};

class Tit4Tat: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                        const { return tStrategyId::tit4tat; };
    virtual bool        InteractWith( StrategyData &, IndId const )             const;
    virtual void        Remember    ( StrategyData &, IndId const, bool const ) const;
};

class EmptyStrategy: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                        const { return tStrategyId::empty; };
	virtual bool        InteractWith( StrategyData &, IndId const )             const { return false; };
	virtual void        Remember    ( StrategyData &, IndId const, bool const ) const {};
};
//lint +e715  
