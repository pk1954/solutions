// config.cpp : 
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include "assert.h"
#include "limits.h"
#include "string.h"
#include <iomanip>
#include "util.h"
#include "config.h"
#include "symtab.h"

std::unordered_map< Config::tId, Config::tConfigItem > Config::m_mapConfigData;

#define DEF_CONFIG_ID( id, init ) { defineConfigId( id, L#id, init ); }

void Config::defineConfigId( Config::tId const id, wstring const & wstrName, long const lInitialValue )
{
    SymbolTable::ScrDefConst( wstrName, static_cast<unsigned long>( id ) );
    Config::m_mapConfigData.insert( std::make_pair( id, lInitialValue ) );
}

void Config::SetDefaultConfiguration( )
{
     // general
   DEF_CONFIG_ID( Config::tId::maxGeneration,   1000 * 1000 );
   DEF_CONFIG_ID( Config::tId::nrOfHistorySlots,      10000 );
   DEF_CONFIG_ID( Config::tId::performanceDisplay, static_cast<long>(tOnOffAuto::automatic) );
   //DEF_CONFIG_ID( Config::tId::historyDisplay,     static_cast<long>(tOnOffAuto::on) );
   DEF_CONFIG_ID( Config::tId::miniGridDisplay,    static_cast<long>(tOnOffAuto::automatic) );
   DEF_CONFIG_ID( Config::tId::showGridPointCoords,       1 );  
   DEF_CONFIG_ID( Config::tId::stdMemSize,                8 );  
   // individual energy          
   DEF_CONFIG_ID( Config::tId::initialEnergy,        10000 );  
   DEF_CONFIG_ID( Config::tId::stdCapacity,          10000 );  
   DEF_CONFIG_ID( Config::tId::extraCapacity,            0 );  
   // abilities         
   DEF_CONFIG_ID( Config::tId::neighborhoodFoodSensitivity, 1 );  
   // energy consumption         
   DEF_CONFIG_ID( Config::tId::energyConsumptionBasicRate,    50 );  
   DEF_CONFIG_ID( Config::tId::energyConsumptionMove,         50 );   
   DEF_CONFIG_ID( Config::tId::energyConsumptionClone,       100 );   
   DEF_CONFIG_ID( Config::tId::energyConsumptionMarry,       100 );  
   DEF_CONFIG_ID( Config::tId::energyConsumptionInteraction,   0 );   
   // energy consuption         
   DEF_CONFIG_ID( Config::tId::energyConsumptionMemSize,       0 );  ///// TODO:  reset to value > 0          
   DEF_CONFIG_ID( Config::tId::energyConsumptionextraCapacity, 0 );  
   // enable/disable activities         
   DEF_CONFIG_ID( Config::tId::moveEnabled,      1 );  
   DEF_CONFIG_ID( Config::tId::cloneEnabled,     1 );  
   DEF_CONFIG_ID( Config::tId::marryEnabled,     1 );  
   DEF_CONFIG_ID( Config::tId::interactEnabled,  1 );  
   DEF_CONFIG_ID( Config::tId::fertilizeEnabled, 1 );  
   DEF_CONFIG_ID( Config::tId::passOnEnabled,    1 );  
   DEF_CONFIG_ID( Config::tId::eatEnabled,       1 );  
   // thresholds for activities         
   DEF_CONFIG_ID( Config::tId::thresholdClone,  500 );  
   DEF_CONFIG_ID( Config::tId::thresholdMarry,  375 );  
   DEF_CONFIG_ID( Config::tId::thresholdMove,   100 );            // should be > energyConsumptionMove          
   DEF_CONFIG_ID( Config::tId::thresholdFert,   300 );  
   DEF_CONFIG_ID( Config::tId::maxEat,         5000 );  
   // food         
   DEF_CONFIG_ID( Config::tId::initialFoodStock,      1000 );  
   DEF_CONFIG_ID( Config::tId::growthRateFood,           5 );  
   DEF_CONFIG_ID( Config::tId::minFood,                 50 );  
   DEF_CONFIG_ID( Config::tId::maxFood,               2000 );  
   DEF_CONFIG_ID( Config::tId::defaultAppetite,         80 );  
   DEF_CONFIG_ID( Config::tId::defaultFertilInvest,     80 );  
   DEF_CONFIG_ID( Config::tId::reserveFood,             20 );  
   DEF_CONFIG_ID( Config::tId::fertilizerYield,          0 );  // percent value. 100 means yield is equal to investment
   DEF_CONFIG_ID( Config::tId::maxFertilizer,        20000 );  
   // interaction         
   DEF_CONFIG_ID( Config::tId::interactionPayOff_R,     50 );  
   DEF_CONFIG_ID( Config::tId::interactionPayOff_S,   -300 );  
   DEF_CONFIG_ID( Config::tId::interactionPayOff_T,    300 );  
   DEF_CONFIG_ID( Config::tId::interactionPayOff_P,    -50 );
   // misc
   DEF_CONFIG_ID( Config::tId::gridWidth,              150 );  
   DEF_CONFIG_ID( Config::tId::gridHeight,             100 );  
   DEF_CONFIG_ID( Config::tId::nrOfNeighbors,            8 );  
   DEF_CONFIG_ID( Config::tId::stripMode,                0 );
   DEF_CONFIG_ID( Config::tId::dimmMode,                 0 );
   DEF_CONFIG_ID( Config::tId::generationDelay,         50 );
}

bool Config::SetConfigValueBoolOp( tId const id, tBoolOp const op )
{
    long const lValue    = GetConfigValue( id );
    bool const bOldState = (lValue != 0);
	bool const bNewState = ApplyOp2( bOldState, op );
	SetConfigValue( id, bNewState ? 1 : 0 );
	return bNewState;
}

void Config::SetConfigValue( tId const id, long const lValue )
{
    m_mapConfigData.at( id ) = lValue;
}

class WrapSetConfigValue : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        unsigned long const ulConfigId = script.ScrReadUlong( );
        Config::tId   const id         = static_cast<Config::tId>( ulConfigId );
        long          const lValue     = script.ScrReadLong();

        Config::SetConfigValue( id, lValue );
    }
};

void Config::DefineConfigWrapperFunctions( )
{
    DEF_FUNC( SetConfigValue );
}
