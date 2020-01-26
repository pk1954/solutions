// GenomeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip> 
#include "EvolutionTypes.h"
#include "config.h"
#include "genome.h"

using namespace std;

static void initConfiguration( )
{ 
    #define CONFIG( id, val ) Config::SetConfigValue( Config::##id, val );

         //    general
    CONFIG( CI_HISTORY_SIZE,                 1000 );
    CONFIG( CI_HISTORY_DISPLAY, Config::MODE_AUTO );
    CONFIG( CI_STD_MEM_SIZE,                    8 );

         // individual energy 
    CONFIG( CI_INITIAL_ENERGY,              10000 );
    CONFIG( CI_STD_CAPACITY,                10000 );
    CONFIG( CI_EXTRA_CAPACITY,                  0 );

         //    energy consumption
    CONFIG( CI_EC_BASIC_RATE,      50 );
    CONFIG( CI_EC_MOVE,           300 );
    CONFIG( CI_EC_CLONE,          100 );
    CONFIG( CI_EC_MARRIAGE,         0 );
    CONFIG( CI_EC_INTERACTION,      0 );

	     //    extra energy consuption
    CONFIG( CI_EC_MEM_SIZE,         1 );
    CONFIG( CI_EC_EXTRA_CAPACITY,   0 );

	     //    thresholds for activities
    CONFIG( CI_MIN_REPRO,     Config::GetConfigValue( Config::CI_EC_BASIC_RATE ) * 10 );
    CONFIG( CI_MIN_MOVE,      Config::GetConfigValue( Config::CI_EC_BASIC_RATE ) *  2 );
    CONFIG( CI_MIN_FERTILIZE, Config::GetConfigValue( Config::CI_EC_BASIC_RATE ) *  2 );
    CONFIG( CI_MAX_EAT,       Config::GetConfigValue( Config::CI_STD_CAPACITY  ) /  2 );

	     // food
    CONFIG( CI_INITIAL_FOOD,            1000 );        
    CONFIG( CI_GROWTH_RATE,                5 ); // in %           
    CONFIG( CI_MIN_FOOD,                  50 );
    CONFIG( CI_MAX_FOOD,                2000 );		      
    CONFIG( CI_DEF_APPETITE,              80 ); // (in energy units)
    CONFIG( CI_DEF_FERT_INVEST,           80 ); // (in energy units)
    CONFIG( CI_RESERVE_FOOD,              20 );
    CONFIG( CI_SPREAD_THRESHOLD,          10 );
    CONFIG( CI_FERTILIZER_YIELD,        1000 );

		 // interaction
    CONFIG( CI_INTERACTION_PAYOFF_R,       30 );
    CONFIG( CI_INTERACTION_PAYOFF_S,     -190 );
    CONFIG( CI_INTERACTION_PAYOFF_T,      190 );
    CONFIG( CI_INTERACTION_PAYOFF_P,      -30 );

    #undef CONFIG
}

static void printGen( Genome const * const pg )
{
    for ( int i = 0; i < NR_ACTIONS; i++ )
	{
		short const sDist = pg->GetDistr( static_cast<ACTION_TYPE>(i) );
        //lint -e747  Significant prototype coercion with setw
        wcout << setw(10) << GetActionTypeName( static_cast<ACTION_TYPE>(i) ) << " " << sDist << endl;
        //lint +e747 
	}
}

//lint -esym( 715, argc )    symbol not referenced
//lint -esym( 715, argv )    symbol not referenced

int _tmain( int const argc, _TCHAR const * const argv[] )
{
    Random random;
    Genome g;
    Genome::InitClass( );
    initConfiguration( );
    g.Initialize( );

    printGen( &g );

	array<bool, NR_ACTIONS> abOptions;

    for ( auto &iOption : abOptions )
        abOptions[ iOption ] = true;

    long alCounter[NR_ACTIONS];

    for ( auto &i : alCounter )
        alCounter[i] = 0L;

    for ( long l = 0; l < 100000; l++ )
    { 
        ACTION_TYPE const at = g.GetOption( abOptions, random );
        alCounter[at]++;
    }

    for ( auto &i : alCounter )
    {
        //lint -e747  Significant prototype coercion with setw
        wcout << setw(10) << GetActionTypeName( static_cast<ACTION_TYPE>(i) ) << " " << alCounter[i] << endl;
        //lint +e747 
    }

    return 0;
}

