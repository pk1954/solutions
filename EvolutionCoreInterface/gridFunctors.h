// gridFunctors.h : 
//

#pragma once

#include "gridPoint.h"
#include "EvolutionTypes.h"

class Strategy_Functor: public GridPointIntensity_Functor 
{
public: 
    Strategy_Functor( ) 
        : GridPointIntensity_Functor( ),
          m_strategy( tStrategyId::empty )
        {}

    void SetStrategy( tStrategyId const s ) { m_strategy = s; }

    bool operator() ( GridPoint const & );
    bool operator() ( GridPoint const &, short const );

private:
    tStrategyId m_strategy;
};

class FoodStock_Functor : public GridPointIntensity_Functor
{
public:
    bool operator() ( GridPoint const & );
    bool operator() ( GridPoint const &, short const );
};

class Fertilizer_Functor : public GridPointIntensity_Functor
{
public:
    bool operator() ( GridPoint const & );
    bool operator() ( GridPoint const &, short const );
};

class Fertility_Functor : public GridPointIntensity_Functor
{
public: 
    bool operator() ( GridPoint const & );
    bool operator() ( GridPoint const &, short const );
};

class MutRate_Functor: public GridPointIntensity_Functor
{
public: 
    bool operator() ( GridPoint const & );
    bool operator() ( GridPoint const &, short const );
};
