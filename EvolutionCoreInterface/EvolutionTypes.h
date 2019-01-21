// EvolutionTypes.h
//

#pragma once

#include <fstream>
#include <iostream>
#include "debug.h"

class GRID_COORD
{
public:
    explicit GRID_COORD( ) 
		: value_(0) 
	{}
		
    constexpr explicit GRID_COORD( short const & value ) 
		: value_(value) 
	{}

    short const& get() const { return value_; }

    GRID_COORD operator++ () { ++value_; return * this; }
    GRID_COORD operator-- () { --value_; return * this; }

    GRID_COORD const operator- () { return GRID_COORD( -value_ ); }

    bool const operator== (GRID_COORD const a) const { return value_ == a.value_; }
    bool const operator!= (GRID_COORD const a) const { return value_ != a.value_; }
    bool const operator<= (GRID_COORD const a) const { return value_ <= a.value_; }
    bool const operator<  (GRID_COORD const a) const { return value_ <  a.value_; }
    bool const operator>= (GRID_COORD const a) const { return value_ >= a.value_; }
    bool const operator>  (GRID_COORD const a) const { return value_ >  a.value_; }

	GRID_COORD const operator+= (GRID_COORD const a) { value_ += a.value_; return * this; }
    GRID_COORD const operator-= (GRID_COORD const a) { value_ -= a.value_; return * this; }
    GRID_COORD const operator%= (GRID_COORD const a) { value_ %= a.value_; return * this; }

    // no * operator!   risk of short overflow

	GRID_COORD const operator/= (int const i) { value_ /= i; return * this; }
    GRID_COORD const operator%= (int const i) { value_ %= i; return * this; }

	bool IsEven( ) const { return value_ % 2 == 0; }
	bool IsOdd ( ) const { return value_ % 2 != 0; }

private:
    short value_;
};

inline GRID_COORD const abs(GRID_COORD const a) { return GRID_COORD( ::abs(a.get()) ); }

inline GRID_COORD const operator+ (GRID_COORD const a, GRID_COORD const b) { GRID_COORD res(a); res += b; return res; }
inline GRID_COORD const operator- (GRID_COORD const a, GRID_COORD const b) { GRID_COORD res(a); res -= b; return res; }

constexpr GRID_COORD operator"" _GRID_COORD( unsigned long long );

std::wostream & operator << ( std::wostream &, GRID_COORD const );

GRID_COORD const MAX_GRID_COORD  = GRID_COORD(255_GRID_COORD);
GRID_COORD const GRID_COORD_NULL = GRID_COORD(SHRT_MIN);

static long const GRID_WIDTH_  = 200;
static long const GRID_HEIGHT_ = 100;
GRID_COORD const GRID_WIDTH  = GRID_COORD(200_GRID_COORD);
GRID_COORD const GRID_HEIGHT = GRID_COORD(100_GRID_COORD);
GRID_COORD const GRID_X_MIN  = GRID_COORD(0_GRID_COORD);
GRID_COORD const GRID_Y_MIN  = GRID_COORD(0_GRID_COORD);
GRID_COORD const GRID_X_MAX  = GRID_WIDTH  - GRID_COORD(1_GRID_COORD);
GRID_COORD const GRID_Y_MAX  = GRID_HEIGHT - GRID_COORD(1_GRID_COORD);

typedef unsigned short MEM_INDEX;
typedef          long  EVO_GENERATION;

static MEM_INDEX const IMEMSIZE_MAX = 9;

//lint -sem(IndId::ResetIndId,initializer)

class IndId
{
public:
	static const IndId NO_INDIVIDUAL;

    IndId( ) { ResetIndId( ); };

    inline IndId & operator++() { ++m_lIdentifier; return *this; }; 
    inline bool    operator==( IndId const & id ) const { return (id.m_lIdentifier == m_lIdentifier); };

    inline bool IsDefined( ) const 
    { 
        return (m_lIdentifier != ID_NO_INDIVIDUAL); 
    };
    
    inline void ResetIndId( )       
    {
        m_lIdentifier = ID_NO_INDIVIDUAL;  
    };

    inline long GetLong( )   const { return m_lIdentifier; };

private:
    static long const ID_NO_INDIVIDUAL = -1;

    long m_lIdentifier;
};

std::wostream & operator << ( std::wostream & , IndId const & );  // implemented in dump.cpp

enum class tStrategyId : unsigned short
{ 
    defect, 
    cooperate, 
    tit4tat, 
//	friedman, 
    nrOfStrategies, 
    random,         // Not really a strategy. Used to signal random fill
    empty,          // Not really a strategy. Used to mark empty slot
    all             // Not really a strategy. Used to mark all strategies
};

static const int NR_STRATEGIES = static_cast<int>( tStrategyId::nrOfStrategies );

enum class tShape : unsigned short
{
    Circle,
    Rect,
	Grid
};

enum class tOrigin : unsigned short
{
    editor,
    cloning,
    marriage
};

enum class tAction : unsigned short
{ 
    move, 
    clone,
    marry,
    interact, 
    eat, 
    fertilize,
	nrOfActionGenes,
	passOn = nrOfActionGenes,
    nrOfActions,
    undefined = nrOfActions
};

static const int NR_ACTIONS      = static_cast<int>( tAction::nrOfActions );
static const int NR_ACTION_GENES = static_cast<int>( tAction::nrOfActionGenes );

enum class tGeneType : unsigned short
{
    appetite,
    fertilInvest,
    memSize,
    thresholdClone,       // minimum available energy for considering CLONE
    thresholdMarry,       // minimum available energy for considering MARRY
    thresholdMove,        // minimum available energy for considering MOVE
    thresholdFertilize,   // minimum available energy for considering FERTILIZE
    maxEat,               // maximum available energy for considering EAT
    cloneDonation,        // amount of energy donated to clone. 0 means nothing (clone dies), SHORT_MAX means all available energy (parent dies)
    nrOfGeneTypes
};

static const int NR_GENES = static_cast<int>( tGeneType::nrOfGeneTypes );

enum class tBrushMode
{
    randomStrat,
    cooperate,
    defect,
    tit4tat,
    noAnimals,
    move,
    mutRate,
    fertility,
    food,
    fertilizer
};

bool IsStrategyBrushMode( tBrushMode const );

enum class tManipulator
{
	set,
	max,
	min,
	add,
	subtract,
	mean
};

enum class tColorObject : unsigned short
{
	individual,
	selection,
	highlight
};

wchar_t const * const GetStrategyName      ( tStrategyId  const );
wchar_t const * const GetShapeName         ( tShape       const );
wchar_t const * const GetOriginName        ( tOrigin      const );
wchar_t const * const GetActionTypeName    ( tAction      const );
wchar_t const * const GetGeneName          ( tGeneType    const );
wchar_t const * const GetBrushModeName     ( tBrushMode   const );
wchar_t const * const GetBrushModeNameShort( tBrushMode   const );
wchar_t const * const GetManipulatorName   ( tManipulator const );
wchar_t const * const GetColorObjectName   ( tColorObject const );

tAction const GetRelatedAction( tGeneType const );
