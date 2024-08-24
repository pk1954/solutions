// EvoConfig.cpp : 
//
// EvoCoreLib

module EvoCoreLib:EvoConfig;

import std;
import Debug;
import RunTime;

using std::wstring;
using std::unordered_map;

unordered_map< EvoConfig::tId, EvoConfig::tConfigItem > EvoConfig::m_mapConfigData;

#define DEF_CONFIG_ID(id, init) { defineConfigId(id, L#id, init); }

void EvoConfig::defineConfigId(EvoConfig::tId const id, wstring const & wstrName, long const lInitialValue)
{
    SymbolTable::ScrDefConst(wstrName, static_cast<unsigned long>(id));
    EvoConfig::m_mapConfigData.insert(std::make_pair(id, lInitialValue));
}

void EvoConfig::SetDefaultConfiguration()
{
     // general
   DEF_CONFIG_ID(EvoConfig::tId::maxGeneration,   1000 * 1000);
   DEF_CONFIG_ID(EvoConfig::tId::nrOfHistorySlots,      30000);
   DEF_CONFIG_ID(EvoConfig::tId::performanceDisplay, static_cast<long>(tOnOffAuto::automatic));
   //DEF_CONFIG_ID(EvoConfig::tId::historyDisplay,     static_cast<long>(tOnOffAuto::on));
   DEF_CONFIG_ID(EvoConfig::tId::miniGridDisplay,    static_cast<long>(tOnOffAuto::automatic));
   DEF_CONFIG_ID(EvoConfig::tId::showGridPointCoords,       1);  
   DEF_CONFIG_ID(EvoConfig::tId::stdMemSize,                8);  
   // individual energy          
   DEF_CONFIG_ID(EvoConfig::tId::initialEnergy,        10000);  
   DEF_CONFIG_ID(EvoConfig::tId::stdCapacity,          10000);  
   DEF_CONFIG_ID(EvoConfig::tId::extraCapacity,            0);  
   // abilities         
   DEF_CONFIG_ID(EvoConfig::tId::neighborhoodFoodSensitivity, 1);  
   // energy consumption         
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionBasicRate,    50);  
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionMove,         50);   
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionClone,       100);   
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionMarry,       100);  
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionInteraction,   0);   
   // energy consuption         
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionMemSize,       0);  ///// TODO:  reset to value > 0          
   DEF_CONFIG_ID(EvoConfig::tId::energyConsumptionextraCapacity, 0);  
   // enable/disable activities         
   DEF_CONFIG_ID(EvoConfig::tId::moveEnabled,      1);  
   DEF_CONFIG_ID(EvoConfig::tId::cloneEnabled,     1);  
   DEF_CONFIG_ID(EvoConfig::tId::marryEnabled,     1);  
   DEF_CONFIG_ID(EvoConfig::tId::interactEnabled,  1);  
   DEF_CONFIG_ID(EvoConfig::tId::fertilizeEnabled, 1);  
   DEF_CONFIG_ID(EvoConfig::tId::passOnEnabled,    1);  
   DEF_CONFIG_ID(EvoConfig::tId::eatEnabled,       1);  
   // thresholds for activities         
   DEF_CONFIG_ID(EvoConfig::tId::thresholdClone,  500);  
   DEF_CONFIG_ID(EvoConfig::tId::thresholdMarry,  375);  
   DEF_CONFIG_ID(EvoConfig::tId::thresholdMove,   100);            // should be > energyConsumptionMove          
   DEF_CONFIG_ID(EvoConfig::tId::thresholdFert,   300);  
   DEF_CONFIG_ID(EvoConfig::tId::maxEat,         5000);  
   // food         
   DEF_CONFIG_ID(EvoConfig::tId::initialFoodStock,      1000);  
   DEF_CONFIG_ID(EvoConfig::tId::growthRateFood,           5);  
   DEF_CONFIG_ID(EvoConfig::tId::minFood,                 50);  
   DEF_CONFIG_ID(EvoConfig::tId::maxFood,               2000);  
   DEF_CONFIG_ID(EvoConfig::tId::defaultAppetite,         80);  
   DEF_CONFIG_ID(EvoConfig::tId::defaultFertilInvest,     80);  
   DEF_CONFIG_ID(EvoConfig::tId::reserveFood,             20);  
   DEF_CONFIG_ID(EvoConfig::tId::fertilizerYield,          0);  // percent value. 100 means yield is equal to investment
   DEF_CONFIG_ID(EvoConfig::tId::maxFertilizer,        20000);  
   // interaction         
   DEF_CONFIG_ID(EvoConfig::tId::interactionPayOff_R,     50);  
   DEF_CONFIG_ID(EvoConfig::tId::interactionPayOff_S,   -300);  
   DEF_CONFIG_ID(EvoConfig::tId::interactionPayOff_T,    300);  
   DEF_CONFIG_ID(EvoConfig::tId::interactionPayOff_P,    -50);
   // misc
   DEF_CONFIG_ID(EvoConfig::tId::gridWidth,              150);  
   DEF_CONFIG_ID(EvoConfig::tId::gridHeight,             100);  
   DEF_CONFIG_ID(EvoConfig::tId::nrOfNeighbors,            8);  
   DEF_CONFIG_ID(EvoConfig::tId::stripMode,                0);
   DEF_CONFIG_ID(EvoConfig::tId::dimmMode,                 0);
}

bool EvoConfig::SetConfigValueBoolOp(tId const id, tBoolOp const op)
{
    long const lValue    = GetConfigValue(id);
    bool const bOldState = (lValue != 0);
	bool const bNewState = ApplyOp2(bOldState, op);
	SetConfigValue(id, bNewState ? 1 : 0);
	return bNewState;
}

void EvoConfig::SetConfigValue(tId const id, long const lValue)
{
    m_mapConfigData.at(id) = lValue;
}

class WrapSetConfigValue : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        unsigned long const ulConfigId = script.ScrReadUlong();
        EvoConfig::tId   const id         = static_cast<EvoConfig::tId>(ulConfigId);
        long          const lValue     = script.ScrReadLong();

        EvoConfig::SetConfigValue(id, lValue);
    }
};

void EvoConfig::DefineConfigWrapperFunctions()
{
    SymbolTable::ScrDefConst(L"SetConfigValue", new WrapSetConfigValue);
}
