// EvolutionTypes.h
//

#pragma once

#include <fstream>
#include <iostream>

typedef unsigned short MEM_INDEX;
typedef          short GRID_COORD;
typedef          long  EVO_GENERATION;

//lint -sem(IndId::ResetIndId,initializer)

class IndId
{
public:
    IndId( ) { ResetIndId( ); };

    inline IndId & operator++() { ++m_lIdentifier; return *this; }; 
    inline bool    operator==( IndId const & id ) const { return (id.m_lIdentifier == m_lIdentifier); };

    inline bool IsDefined ( ) const 
    { 
        return (m_lIdentifier != ID_NO_INDIVIDUAL); 
    };
    
    inline void ResetIndId( )       
    {
        m_lIdentifier  = ID_NO_INDIVIDUAL;  
    };

    inline long GetLong ( )   const { return m_lIdentifier; };

private:
    static long const ID_NO_INDIVIDUAL = -1;

    long m_lIdentifier;
};

std::wostream & operator << ( std::wostream & , IndId const & );  // implemented in dump.cpp

enum class tStrategyId : unsigned short
{ 
    defectAlways, 
    cooperateAlways, 
    tit4tat, 
//	friedman, 
    nrOfStrategies, 
    random,         // Not really a strategy. Used to signal random fill
    empty           // Not really a strategy. Used to mark empty slot
};

static const int NR_STRATEGIES = static_cast<int>( tStrategyId::nrOfStrategies );

enum class tShape : unsigned short
{
    Circle,
    Rect
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
    nrOfActions,
    undefined = nrOfActions
};

static const int NR_ACTIONS = static_cast<int>( tAction::nrOfActions );

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
    randomStrategy,
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

wchar_t const * const GetStrategyName      ( tStrategyId const );
wchar_t const * const GetShapeName         ( tShape      const );
wchar_t const * const GetOriginName        ( tOrigin     const );
wchar_t const * const GetActionTypeName    ( tAction     const );
wchar_t const * const GetGeneName          ( tGeneType   const );
wchar_t const * const GetBrushModeName     ( tBrushMode  const );
wchar_t const * const GetBrushModeNameShort( tBrushMode  const );

tAction const GetRelatedAction( tGeneType const );
