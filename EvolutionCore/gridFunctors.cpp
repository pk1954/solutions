// gridFunctors.cpp :
//

#include "stdafx.h"
#include "gridPoint.h"
#include "gridFunctors.h"
#include "grid_model.h"

bool Strategy_Functor ::operator() ( GridPoint const & gp )                   { GetGrid()->EditSetStrategy( gp, GetBrushIntensity( ), m_strategy ); return false; }
bool Strategy_Functor ::operator() ( GridPoint const & gp, short const sVal ) { GetGrid()->EditSetStrategy( gp, sVal,                 m_strategy ); return false; }

bool Fertilizer_Functor::operator() ( GridPoint const & gp )                   { GetGrid( )->IncFertilizer( gp, GetBrushIntensity( ) );             return false; };
bool Fertilizer_Functor::operator() ( GridPoint const & gp, short const sVal ) { GetGrid( )->IncFertilizer( gp, sVal );                             return false; };

bool FoodStock_Functor::operator() ( GridPoint const & gp )                   { GetGrid( )->IncFoodStock( gp, GetBrushIntensity( ) );               return false; };
bool FoodStock_Functor::operator() ( GridPoint const & gp, short const sVal ) { GetGrid( )->IncFoodStock( gp, sVal );                               return false; };

bool Fertility_Functor::operator() ( GridPoint const & gp )                   { GetGrid()->IncFertility( gp, GetBrushIntensity( ) );                return false; };
bool Fertility_Functor::operator() ( GridPoint const & gp, short const sVal ) { GetGrid()->IncFertility( gp, sVal );                                return false; };

bool MutRate_Functor  ::operator() ( GridPoint const & gp )                   { GetGrid()->IncMutationRate( gp, GetBrushIntensity( ) );             return false; };
bool MutRate_Functor  ::operator() ( GridPoint const & gp, short const sVal ) { GetGrid()->IncMutationRate( gp, sVal );                             return false; };
