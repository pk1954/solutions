// gridFunctors.cpp :
//

#include "stdafx.h"
#include "gridPoint.h"
#include "gridFunctors.h"
#include "grid_model.h"

void Strategy_Functor ::operator() ( GridPoint const & gp )                   { GetGrid()->EditSetStrategy( gp, GetBrushIntensity( ), m_strategy ); }
void Strategy_Functor ::operator() ( GridPoint const & gp, short const sVal ) { GetGrid()->EditSetStrategy( gp, sVal,                 m_strategy ); }

void Fertilizer_Functor::operator() ( GridPoint const & gp )                   { GetGrid( )->IncFertilizer( gp, GetBrushIntensity( ) ); };
void Fertilizer_Functor::operator() ( GridPoint const & gp, short const sVal ) { GetGrid( )->IncFertilizer( gp, sVal ); };

void FoodStock_Functor::operator() ( GridPoint const & gp )                   { GetGrid( )->IncFoodStock( gp, GetBrushIntensity( ) ); };
void FoodStock_Functor::operator() ( GridPoint const & gp, short const sVal ) { GetGrid( )->IncFoodStock( gp, sVal ); };

void Fertility_Functor::operator() ( GridPoint const & gp )                   { GetGrid()->IncFertility( gp, GetBrushIntensity( ) ); };
void Fertility_Functor::operator() ( GridPoint const & gp, short const sVal ) { GetGrid()->IncFertility( gp, sVal ); };

void MutRate_Functor  ::operator() ( GridPoint const & gp )                   { GetGrid()->IncMutationRate( gp, GetBrushIntensity( ) ); };
void MutRate_Functor  ::operator() ( GridPoint const & gp, short const sVal ) { GetGrid()->IncMutationRate( gp, sVal ); };
