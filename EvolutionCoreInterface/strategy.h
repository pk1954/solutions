// strategy.h : 
//
//EvolutionCoreInterface

#pragma once
#include <array>
#include <functional>
#include "EvolutionTypes.h"

class StrategyData;

class Strategy
{
public:
	enum class Id : unsigned short
	{ 
		defect, 
		cooperate, 
		tit4tat, 
	//	friedman, 
		count, 
		random,         // Not really a strategy. Used to signal random fill
		empty,          // Not really a strategy. Used to mark empty slot
		all             // Not really a strategy. Used to mark all strategies
	};

	static const int COUNT = static_cast<int>( Id::count );

	virtual Id   GetStrategyId( )                                         const = 0;
    virtual bool InteractWith( StrategyData &, IND_ID const )             const = 0;
    virtual void Remember    ( StrategyData &, IND_ID const, bool const ) const = 0;

	static wchar_t const * const GetName( Id const );

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
		{
            func( static_cast<Id>(index) );
		}
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
	virtual void Remember    ( StrategyData &, IND_ID const, bool const ) const { };
};
