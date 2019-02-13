// strategy.h : 
//

#pragma once
#include <array>
#include "random.h"
#include "EvolutionTypes.h"
#include "IndividualId.h"

class StrategyData
{
public:
    StrategyData( );

    void SetMemorySize( MEM_INDEX const );

    // display information

    IndividualId GetMemEntry( MEM_INDEX const ) const;	
    MEM_INDEX    GetMemSize ( )                 const { return m_memSize; };
    MEM_INDEX    GetMemUsed ( )                 const { return m_memUsed; };

    static MEM_INDEX    GetMaxPartnerMemory( )                 { return m_uiMaxPartnerMemory; };
    static unsigned int GetNrInteractionsWithKnownCulprit( )   { return m_uiNrInteractionsWithKnownCulprit; };
    static unsigned int GetNrInteractionsWithUnknownCulprit( ) { return m_uiNrInteractionsWithUnknownCulprit; };

	MEM_INDEX FindInList( IndividualId const & );
    void      AddToList ( IndividualId const & );
    void      RemoveFromList( MEM_INDEX const );

private:
    static MEM_INDEX    m_uiMaxPartnerMemory;
    static unsigned int m_uiNrInteractionsWithKnownCulprit;
    static unsigned int m_uiNrInteractionsWithUnknownCulprit;

    MEM_INDEX m_memUsed;   // number of occopied slots.     
    MEM_INDEX m_memSize;   // number of usable slots.    m_memUsed <= m_memSize <= IMEMSIZE_MAX

    std::array< IndividualId, IMEMSIZE_MAX > m_aIdBadGuys;

	IndividualId getBadGuyId( MEM_INDEX const index ) const
	{
		return m_aIdBadGuys[ index.GetValue() ];
	}
};

class Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                               const = 0;
    virtual bool        InteractWith( StrategyData &, IndividualId const )             const = 0;
    virtual void        Remember    ( StrategyData &, IndividualId const, bool const ) const = 0;
};

class DefectAlways: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                                    const { return tStrategyId::defect; };
    virtual bool        InteractWith( StrategyData &mData, IndividualId const lPartnerId )  const { return false; };
    virtual void        Remember    ( StrategyData &mData, IndividualId const, bool const ) const { };
};

class CooperateAlways: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                                    const { return tStrategyId::cooperate; };
    virtual bool        InteractWith( StrategyData &mData, IndividualId const lPartnerId )  const { return true; };
    virtual void        Remember    ( StrategyData &mData, IndividualId const, bool const ) const { };
};

class Tit4Tat: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                               const { return tStrategyId::tit4tat; };
    virtual bool        InteractWith( StrategyData &, IndividualId const )             const;
    virtual void        Remember    ( StrategyData &, IndividualId const, bool const ) const;
};

class EmptyStrategy: public Strategy
{
public:
    virtual tStrategyId GetStrategyId( )                                               const { return tStrategyId::empty; };
	virtual bool        InteractWith( StrategyData &, IndividualId const )             const { return false; };
	virtual void        Remember    ( StrategyData &, IndividualId const, bool const ) const {};
};
